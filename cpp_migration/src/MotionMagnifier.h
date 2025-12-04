#ifndef MOTIONMAGNIFIER_H
#define MOTIONMAGNIFIER_H

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief Clase principal para el algoritmo de Magnificación de Movimiento Euleriano.
 * Traduce la lógica de pyrtools y filtrado temporal de Python a C++ optimizado.
 */
class MotionMagnifier {
public:
    MotionMagnifier();
    ~MotionMagnifier();

    /**
     * @brief Inicializa los parámetros del algoritmo.
     * @param width Ancho del frame.
     * @param height Alto del frame.
     * @param fps Frames por segundo del video.
     * @param alpha Factor de amplificación.
     * @param lambda_c Longitud de onda de corte espacial.
     * @param fl Frecuencia de corte baja (Hz).
     * @param fh Frecuencia de corte alta (Hz).
     */
    void init(int width, int height, double fps, double alpha, double lambda_c, double fl, double fh);

    /**
     * @brief Procesa un frame individual.
     * @param input Frame de entrada (BGR o Grayscale).
     * @param output Frame de salida magnificado.
     */
    void processFrame(const cv::Mat& input, cv::Mat& output);

    // Setters para actualizar parámetros en tiempo real
    void setAlpha(double alpha);
    void setFrequencies(double fl, double fh);

private:
    // Parámetros
    int m_width, m_height;
    double m_fps;
    double m_alpha;
    double m_lambda_c;
    double m_fl, m_fh;

    // Estado interno
    bool m_initialized;
    
    // Buffers para filtrado temporal (IIR)
    // Almacenamos pirámides anteriores para el filtro recursivo
    std::vector<cv::Mat> m_lowpass1;
    std::vector<cv::Mat> m_lowpass2;
    std::vector<cv::Mat> m_pyr_prev;

    // Coeficientes del filtro IIR (Butterworth)
    // Se calculan al iniciar o cambiar frecuencias
    struct FilterCoeffs {
        double a[2]; // Denominador
        double b[2]; // Numerador
    };
    FilterCoeffs m_low_coeffs, m_high_coeffs;

    /**
     * @brief Construye una pirámide Laplaciana.
     */
    void buildLaplacianPyramid(const cv::Mat& img, std::vector<cv::Mat>& pyramid, int levels);

    /**
     * @brief Reconstruye la imagen desde la pirámide.
     */
    void reconPyramid(const std::vector<cv::Mat>& pyramid, cv::Mat& output);

    /**
     * @brief Calcula coeficientes de filtro Butterworth de primer orden.
     */
    FilterCoeffs calculateButterworthCoeffs(double cutoffFreq, double samplingRate, bool isLowPass);
};

#endif // MOTIONMAGNIFIER_H
