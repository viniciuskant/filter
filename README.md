# Audio Filters for Rofofo

This repository contains a collection of audio filters implemented in **C** and **Python**, aimed at processing and cleaning audio signals, enhancing **human voice** and reducing unwanted noise. These implementations will be used as part of the [Rofofo](https://github.com/Unicamp-Odhin/Rofofo) project, which focuses on developing hardware for real-time audio processing.

## Objective

Develop efficient audio filters that can be used to:
- Highlight human speech in noisy environments
- Reduce background noise (static, impulsive, etc.)
- Prepare audio for pattern recognition via embedded AI
- Serve as a foundation for hardware acceleration (FPGA)

## Applications

- Embedded voice assistants
- Industrial anomaly detection
- More robust human-machine interfaces
- Intelligent audio-based automation

## Technologies Used

- **C**: for low-level optimizations and future FPGA implementation
- **Python**: for prototyping, analysis, and filter validation
- **Libraries**:
    - `numpy`, `scipy`, `librosa` (Python)
    - `fftw`, `portaudio` (C, optional)

## Repository Structure
`samples/`
 - `I2S/` 
     - `12200hz/`
     - `3051hz/`
     - `6103hz/`
     - `README.md`
 - `PDM/`
     - `16bits/`
     - `8bits/`
     - `8bits_2/`
     - `README.md`

## I2S - MSM261S4030H0 Microphone

This project utilizes the **MSM261S4030H0 microphone** with the **I2S interface**, enabling efficient audio data capture and processing. The **MSM261S4030H0 microphone** is the I2S microphone used in this project.

The datasheet for the MSM261S4030H0 microphone can be found at: `documentation/microphone-i2s.pdf`.

## PDM Microphone
This project also incorporates a **PDM microphone** as found on the **Nexys4** development board. The PDM microphone enables audio signal acquisition with Pulse Density Modulation, which is then processed for various applications.

For more details, refer to the Nexys4 documentation: `documentation/microphone-pdm.pdf`.
