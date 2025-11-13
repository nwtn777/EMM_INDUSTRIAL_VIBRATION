# EMM - Amplificación de Movimiento para Análisis de Vibraciones Industriales

## 📋 Descripción General

**EMM_INDUSTRIAL_VIBRATION** es una suite completa de herramientas de software diseñada para detectar, amplificar, analizar y visualizar vibraciones industriales mínimas. El proyecto utiliza técnicas avanzadas de amplificación de movimiento, análisis de Fourier (FFT) y procesamiento de video en tiempo real para identificar patrones de vibración que no son perceptibles al ojo humano.

## 🎯 Características Principales

### 1. **Motion Magnification GUI** (Amplificación de Movimiento)
- **Amplificación automática de movimiento** utilizando pirámides Laplacianas (pyrtools)
- **Detección de ROI (Región de Interés)** mediante clic del usuario en tiempo real
- **Parámetros ajustables**:
  - **Alpha (α)**: Control de magnitud de amplificación (50-300)
  - **Lambda (λ)**: Longitud de onda de la amplificación (20-120)
  - **Frequency Range**: Rango de frecuencias a amplificar (fl, fh)
  - **Skip Frames**: Salto de fotogramas para optimización de rendimiento

- **Optimización automática de parámetros**:
  - Búsqueda automática de los mejores valores de alpha y lambda
  - Métrica de optimización basada en energía de movimiento
  - Auto-tuning durante periodos configurables

- **Análisis de Vibración Multi-Método**:
  - Método de **Brillo (Intensidad)**: Análisis de cambios de luminancia
  - Método de **Flujo Óptico**: Detección de movimiento mediante optical flow

- **Visualización en Tiempo Real**:
  - Captura de video desde cámara web
  - Visualización simultánea de video original y amplificado
  - Gráficas en vivo de espectro de frecuencias (FFT)
  - Gráficas de amplitud en el dominio del tiempo

- **Registro de Datos**:
  - Exportación automática a CSV de historiales de vibración
  - Guardado de amplitudes en buffer circular
  - Registro temporal con timestamps
  - Descarga manual de historiales

### 2. **Vibration FFT Analyzer** (Analizador de Frecuencias)
- **Carga de archivos CSV** con datos de vibración
- **Análisis espectral mediante FFT** (Transformada Rápida de Fourier)
- **Filtrado avanzado**:
  - Filtro paso-alto (high-pass) configurable
  - Eliminación de offset DC y ruido de baja frecuencia
  - Parámetros ajustables para corte de frecuencia

- **Parámetros de análisis personalizables**:
  - Frecuencia de muestreo (sampling frequency)
  - Tiempo de corte (cut-off time)
  - Frecuencia máxima a mostrar
  - Selección de columna de datos

- **Visualización dual**:
  - Gráfica de señal en tiempo
  - Espectro de frecuencias (FFT)

### 3. **Generador de Reportes Estadísticos** (Report Generator)
- **Análisis estadístico automático**:
  - Media, desviación estándar, mínimo, mediana, máximo
  - Cálculo de estadísticas por columna

- **Generación de gráficas**:
  - Gráficas de señal temporal
  - Espectros de frecuencia (FFT)
  - Identificación de 3 frecuencias dominantes

- **Exportación a PDF**:
  - Reportes profesionales en formato PDF
  - Documentación completa con gráficas incrustadas

### 4. **Launcher (Gestor de Dependencias)**
- **Verificador automático de dependencias**
- **Instalación asistida** de paquetes faltantes
- **Interfaz gráfica intuitiva** para gestión de dependencias
- **Ejecución directa** de la GUI completa

## 🏗️ Arquitectura del Proyecto

```
EMM_INDUSTRIAL_VIBRATION/
├── motion_magnification_gui.py      # Aplicación principal de amplificación
├── vibration_analyzer_gui.py         # Analizador FFT
├── reporte_estadistico.py            # Generador de reportes PDF
├── launcher.py                       # Gestor de dependencias
├── config.json                       # Configuración de parámetros
├── requirements.txt                  # Dependencias del proyecto
├── run_gui.bat                       # Script para ejecutar en Windows
└── README.md                         # Este archivo
```

## 📦 Dependencias

| Paquete | Función |
|---------|---------|
| **opencv-python** | Procesamiento de video |
| **numpy** | Operaciones numéricas |
| **matplotlib** | Visualización de gráficas |
| **scipy** | Procesamiento de señales |
| **scikit-image** | Procesamiento de imágenes |
| **pyrtools** | Pirámides Laplacianas |
| **pillow** | Manipulación de imágenes |
| **pandas** | Análisis de datos |
| **fpdf** | Generación de PDF |

## 🚀 Instalación y Uso

### Método 1: Ejecución con Launcher (Recomendado)
```bash
python launcher.py
```
El launcher verificará las dependencias e instalará las faltantes automáticamente.

### Método 2: Ejecución directa (requiere instalación previa)
```bash
# Instalar dependencias
pip install -r requirements.txt

# Ejecutar la GUI de amplificación de movimiento
python motion_magnification_gui.py

# Ejecutar el analizador FFT
python vibration_analyzer_gui.py
```

### Método 3: Windows
```bash
run_gui.bat
```

## ⚙️ Configuración

Los parámetros de configuración se encuentran en `config.json`:

```json
{
  "default_settings": {
    "camera_id": 0,              # ID de cámara (0 = cámara predeterminada)
    "fps": 10.0,                 # Fotogramas por segundo
    "alpha": 200.0,              # Magnitud de amplificación
    "lambda_c": 120.0,           # Longitud de onda
    "fl": 0.07,                  # Frecuencia baja del rango
    "fh": 3.0,                   # Frecuencia alta del rango
    "buffer_size": 300,          # Tamaño del buffer circular
    "auto_tune_duration": 5      # Duración del auto-tuning (segundos)
  },
  "gui_settings": {
    "window_width": 1200,
    "window_height": 800,
    "graph_update_interval": 100 # ms
  },
  "file_settings": {
    "csv_output_dir": "historiales",
    "auto_save": true
  }
}
```

## 🎮 Guía de Uso

### Amplificación de Movimiento
1. Inicia `motion_magnification_gui.py`
2. Selecciona la **ROI (Región de Interés)** haciendo clic en el video
3. Ajusta los parámetros:
   - **Alpha**: Mayor valor = mayor amplificación
   - **Lambda**: Longitud de onda a amplificar
4. Observa las vibraciones amplificadas en tiempo real
5. Descarga los datos capturados como CSV

### Análisis FFT
1. Inicia `vibration_analyzer_gui.py`
2. Carga un archivo CSV de datos de vibración
3. Configura los parámetros de análisis (frecuencia de muestreo, etc.)
4. Activa el filtro paso-alto si es necesario
5. Visualiza el espectro de frecuencias

### Generación de Reportes
1. En la pestaña "Reporte Estadístico" de la GUI de amplificación
2. Selecciona uno o más archivos CSV
3. El sistema genera automáticamente reportes PDF con:
   - Estadísticas detalladas
   - Gráficas de señales
   - Análisis espectral
   - Frecuencias dominantes

## 📊 Funcionalidades Avanzadas

### Optimización Automática de Parámetros
- **Búsqueda inteligente** de valores óptimos de alpha y lambda
- **Métrica de energía** para evaluar calidad de amplificación
- **Reducción de tiempo de tuning** manual

### Análisis Multi-Método
- Comparación entre diferentes técnicas de detección
- Selección del método más efectivo para cada aplicación

### Exportación de Datos
- **CSV**: Para análisis posterior en Excel, MATLAB, Python
- **PDF**: Reportes profesionales con estadísticas y gráficas
- **Visualización en vivo**: Monitoreo instantáneo de vibraciones

## 🔧 Requisitos del Sistema

- **Python**: 3.7 o superior
- **Sistema Operativo**: Windows, macOS, Linux
- **RAM**: Mínimo 4GB (recomendado 8GB)
- **Procesador**: Dual-core mínimo (quad-core recomendado)
- **Cámara**: Obligatorio

## 📝 Notas Técnicas

- La **amplificación de movimiento** utiliza pirámides Laplacianas para descomponer la imagen en múltiples escalas
- El **análisis FFT** emplea la transformada rápida de Fourier para identificar componentes de frecuencia
- Los **filtros paso-alto** eliminan el offset DC y el ruido de baja frecuencia
- El **buffer circular** mantiene un histórico configurable de mediciones

## 🐛 Solución de Problemas

### ImportError: pyrtools
```bash
pip install pyrtools
```

### La cámara no se abre
- Verifica que la cámara esté conectada
- Cambia `camera_id` en `config.json`

### Bajo rendimiento
- Reduce el `fps` en la configuración
- Aumenta `skip_frames` para procesar menos fotogramas
- Reduce el tamaño de la ventana

## 📄 Licencia

Ver archivo `LICENSE`

## 👤 Autor

**nwtn777** - Proyecto desarrollado para análisis de vibraciones industriales

## 🤝 Contribuciones

Las contribuciones son bienvenidas. Por favor:
1. Fork el proyecto
2. Crea una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Abre un Pull Request

---

**Última actualización**: Noviembre 2025
