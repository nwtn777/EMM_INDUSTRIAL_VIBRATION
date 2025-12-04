#include "MotionMagnifier.h"
#include <cmath>
#include <iostream>

// Constante PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

MotionMagnifier::MotionMagnifier() : m_initialized(false) {
}

MotionMagnifier::~MotionMagnifier() {
}

void MotionMagnifier::init(int width, int height, double fps, double alpha, double lambda_c, double fl, double fh) {
    m_width = width;
    m_height = height;
    m_fps = fps;
    m_alpha = alpha;
    m_lambda_c = lambda_c;
    m_fl = fl;
    m_fh = fh;

    // Calcular coeficientes de filtros
    // Nota: Esta es una simplificación. En una implementación real, usaríamos una librería DSP o
    // implementaríamos la transformación bilineal completa para biquads.
    // Aquí simulamos la estructura de coeficientes que usa el código Python (scipy.signal.butter)
    m_low_coeffs = calculateButterworthCoeffs(fl, fps, true);
    m_high_coeffs = calculateButterworthCoeffs(fh, fps, true); // Python usa lowpass para ambos límites en el filtro pasa-banda

    // Reiniciar buffers
    m_lowpass1.clear();
    m_lowpass2.clear();
    m_pyr_prev.clear();
    
    m_initialized = true;
}

void MotionMagnifier::setAlpha(double alpha) {
    m_alpha = alpha;
}

void MotionMagnifier::setFrequencies(double fl, double fh) {
    m_fl = fl;
    m_fh = fh;
    m_low_coeffs = calculateButterworthCoeffs(fl, m_fps, true);
    m_high_coeffs = calculateButterworthCoeffs(fh, m_fps, true);
}

void MotionMagnifier::processFrame(const cv::Mat& input, cv::Mat& output) {
    if (!m_initialized || input.empty()) return;

    // 1. Convertir a float y escala de grises si es necesario
    cv::Mat gray;
    if (input.channels() == 3) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = input.clone();
    }
    gray.convertTo(gray, CV_32F, 1.0/255.0);

    // 2. Construir Pirámide Laplaciana
    // El número de niveles depende del tamaño de la imagen. 
    // OpenCV pyrDown reduce a la mitad.
    int levels = 5; // Valor típico, podría ser dinámico
    std::vector<cv::Mat> pyramid;
    buildLaplacianPyramid(gray, pyramid, levels);

    // 3. Inicializar buffers si es el primer frame
    if (m_lowpass1.empty()) {
        m_lowpass1 = pyramid; // Copia profunda
        m_lowpass2 = pyramid;
        m_pyr_prev = pyramid;
        output = input.clone(); // Primer frame no se magnifica
        return;
    }

    // 4. Filtrado Temporal (Paso a paso por nivel de pirámide)
    std::vector<cv::Mat> filtered(levels);
    
    for (int i = 0; i < levels; ++i) {
        // Implementación del filtro IIR temporal (equivalente al código Python)
        // lowpass1 = (-high_b[1]*lowpass1 + high_a[0]*pyr + high_a[1]*pyr_prev) / high_b[0]
        // Esta es la ecuación de diferencias. 
        // En C++ con OpenCV, usamos operaciones matriciales:
        
        // Nota: Simplificación para el ejemplo. Asumimos coeficientes normalizados (a[0]=1)
        // y estructura simple.
        
        // Actualizar lowpass1 (Filtro High Cutoff)
        // m_lowpass1[i] = m_lowpass1[i] * -m_high_coeffs.b[1] ... (lógica completa requerida)
        
        // Para este esqueleto, haremos una diferencia simple para demostrar el flujo
        // (Esto es equivalente a un filtro pasa-altas muy básico)
        filtered[i] = pyramid[i] - m_pyr_prev[i]; 
        
        // Actualizar histórico
        m_pyr_prev[i] = pyramid[i].clone();
    }

    // 5. Amplificación
    // Aplicar alpha a los niveles filtrados, atenuando los niveles extremos si es necesario
    double currAlpha = m_alpha;
    // Lógica de atenuación espacial basada en lambda_c...
    
    for (int i = 0; i < levels; ++i) {
        // Aplicar ganancia
        filtered[i] *= currAlpha;
    }

    // 6. Colapsar Pirámide (Reconstrucción)
    cv::Mat motion;
    reconPyramid(filtered, motion);

    // 7. Sumar al original y saturar
    // Redimensionar motion si hubo problemas de tamaño en la pirámide
    if (motion.size() != gray.size()) {
        cv::resize(motion, motion, gray.size());
    }

    cv::Mat result = gray + motion;
    
    // Convertir de vuelta a 8-bit
    result.convertTo(output, CV_8U, 255.0);
    
    // Si la entrada era color, podríamos aplicar la magnificación solo a la luminancia (Y en YUV)
    // y luego reconvertir, o simplemente devolver en gris por ahora.
    if (input.channels() == 3) {
        cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    }
}

void MotionMagnifier::buildLaplacianPyramid(const cv::Mat& img, std::vector<cv::Mat>& pyramid, int levels) {
    pyramid.clear();
    cv::Mat current = img.clone();
    std::vector<cv::Mat> gaussianPyr;
    
    // Construir pirámide Gaussiana
    gaussianPyr.push_back(current);
    for (int i = 0; i < levels; ++i) {
        cv::Mat next;
        cv::pyrDown(current, next);
        gaussianPyr.push_back(next);
        current = next;
    }
    
    // Construir Laplaciana (Diferencia de Gaussianas)
    for (int i = 0; i < levels; ++i) {
        cv::Mat up;
        cv::pyrUp(gaussianPyr[i+1], up, gaussianPyr[i].size());
        cv::Mat lap;
        cv::subtract(gaussianPyr[i], up, lap);
        pyramid.push_back(lap);
    }
    // El último nivel es la imagen gaussiana más pequeña (residuo)
    pyramid.push_back(gaussianPyr[levels]);
}

void MotionMagnifier::reconPyramid(const std::vector<cv::Mat>& pyramid, cv::Mat& output) {
    if (pyramid.empty()) return;
    
    int levels = pyramid.size() - 1;
    cv::Mat current = pyramid[levels]; // Empezar desde el residuo
    
    for (int i = levels - 1; i >= 0; --i) {
        cv::Mat up;
        cv::pyrUp(current, up, pyramid[i].size());
        cv::add(up, pyramid[i], current);
    }
    output = current;
}

MotionMagnifier::FilterCoeffs MotionMagnifier::calculateButterworthCoeffs(double cutoffFreq, double samplingRate, bool isLowPass) {
    // Implementación dummy. En producción usaríamos fórmulas estándar de DSP.
    FilterCoeffs c;
    c.a[0] = 1.0; c.a[1] = 0.0;
    c.b[0] = 1.0; c.b[1] = 0.0;
    return c;
}
