# hardware-software-co-design-of-short-time-fourier-transform


# Introduction

* Short Time Fourier Transform is an important tool in signal processing- It is one of the tools for generating a spectogram. [1]
* It makes it possible to inspect time-frequency domain of signals makes it essential
*  Enabling the visualization of how the signal varies with
Applications include
* In electrocardiographs to detect abnormal heart rates, 
* used in seizure prediction in electroencephalogram signals.
* Employed in machine learning
Hardware acceleration of STFT offer great benefits in terms of improving the latency, cost, energy utilization, computational speed

# Project Overview
The hardware/software implementation achieves a 186% improvement in speed when the FFT is accelerated using AXI-Lite interface and a 1,080% with AXI-Stream
