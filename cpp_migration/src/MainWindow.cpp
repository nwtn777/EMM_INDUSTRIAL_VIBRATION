#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_isProcessing(false)
{
    // 1. Configurar UI
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Video Display
    m_videoLabel = new QLabel("Video Output");
    m_videoLabel->setAlignment(Qt::AlignCenter);
    m_videoLabel->setMinimumSize(640, 480);
    m_videoLabel->setStyleSheet("background-color: black; color: white;");
    mainLayout->addWidget(m_videoLabel);
    
    // Controles
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    
    m_startBtn = new QPushButton("Iniciar");
    connect(m_startBtn, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    controlsLayout->addWidget(m_startBtn);
    
    m_stopBtn = new QPushButton("Detener");
    m_stopBtn->setEnabled(false);
    connect(m_stopBtn, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    controlsLayout->addWidget(m_stopBtn);
    
    controlsLayout->addWidget(new QLabel("Alpha:"));
    m_alphaSlider = new QSlider(Qt::Horizontal);
    m_alphaSlider->setRange(0, 200);
    m_alphaSlider->setValue(50);
    connect(m_alphaSlider, &QSlider::valueChanged, this, &MainWindow::onAlphaChanged);
    controlsLayout->addWidget(m_alphaSlider);
    
    mainLayout->addLayout(controlsLayout);
    
    // 2. Configurar Timer para loop de video
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::updateFrame);
}

MainWindow::~MainWindow() {
    if (m_cap.isOpened()) m_cap.release();
}

void MainWindow::onStartClicked() {
    // Abrir cámara (ID 0)
    m_cap.open(0);
    if (!m_cap.isOpened()) {
        qDebug() << "Error: No se pudo abrir la cámara";
        return;
    }
    
    // Inicializar magnificador
    double fps = m_cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0) fps = 30.0;
    
    m_magnifier.init(
        m_cap.get(cv::CAP_PROP_FRAME_WIDTH),
        m_cap.get(cv::CAP_PROP_FRAME_HEIGHT),
        fps,
        m_alphaSlider->value(),
        80.0, // Lambda
        0.05, // fl
        1.0   // fh
    );
    
    m_isProcessing = true;
    m_startBtn->setEnabled(false);
    m_stopBtn->setEnabled(true);
    
    // Iniciar loop (30ms ~ 33 FPS)
    m_timer->start(30);
}

void MainWindow::onStopClicked() {
    m_timer->stop();
    m_cap.release();
    m_isProcessing = false;
    m_startBtn->setEnabled(true);
    m_stopBtn->setEnabled(false);
    m_videoLabel->clear();
    m_videoLabel->setText("Sistema Detenido");
}

void MainWindow::onAlphaChanged(int value) {
    m_magnifier.setAlpha(static_cast<double>(value));
}

void MainWindow::updateFrame() {
    if (!m_cap.isOpened()) return;
    
    cv::Mat frame;
    m_cap >> frame;
    if (frame.empty()) return;
    
    // Procesar frame
    cv::Mat processed;
    m_magnifier.processFrame(frame, processed);
    
    // Mostrar en GUI
    if (!processed.empty()) {
        QImage qimg = matToQImage(processed);
        m_videoLabel->setPixmap(QPixmap::fromImage(qimg).scaled(
            m_videoLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QImage MainWindow::matToQImage(const cv::Mat& mat) {
    // Conversión básica de OpenCV BGR a Qt RGB
    if (mat.type() == CV_8UC3) {
        const uchar *qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
        return img.rgbSwapped(); // OpenCV usa BGR, Qt usa RGB
    } else if (mat.type() == CV_8UC1) {
        const uchar *qImageBuffer = (const uchar*)mat.data;
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img;
    }
    return QImage();
}
