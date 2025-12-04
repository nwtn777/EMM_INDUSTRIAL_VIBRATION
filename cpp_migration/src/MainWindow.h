#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "MotionMagnifier.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateFrame(); // Slot llamado por el timer para procesar video
    void onStartClicked();
    void onStopClicked();
    void onAlphaChanged(int value);

private:
    // UI Elements
    QLabel *m_videoLabel;
    QPushButton *m_startBtn;
    QPushButton *m_stopBtn;
    QSlider *m_alphaSlider;
    
    // Video Processing
    cv::VideoCapture m_cap;
    QTimer *m_timer;
    MotionMagnifier m_magnifier;
    
    bool m_isProcessing;
    
    // Helper para convertir cv::Mat a QImage
    QImage matToQImage(const cv::Mat& mat);
};

#endif // MAINWINDOW_H
