# Motion Detection with AI Thinker ESP32-CAM

This project demonstrates a motion detection system using the AI Thinker ESP32-CAM module. The system captures video frames, analyzes them for changes, and detects motion based on a configurable threshold. It is lightweight, efficient, and ideal for IoT applications.

## Features

- **Real-time Motion Detection**: Detects motion by comparing consecutive video frames.
- **Configurable Parameters**: Adjust motion sensitivity and thresholds.
- **Compact and Efficient**: Designed for the ESP32-CAM module with minimal resource usage.
- **Serial Feedback**: Outputs motion detection alerts and scores via the serial monitor.

## Hardware Requirements

- AI Thinker ESP32-CAM module
- USB-to-Serial adapter for programming
- 5V power supply

## Software Requirements

- Arduino IDE (with ESP32 board support installed)
- Required libraries: `esp_camera.h`

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/LansariFedi/espcam-motion-detection.git
cd espcam-motion-detection
```

### 2. Setup Arduino IDE

1. Install the ESP32 board package in Arduino IDE.
2. Open the `espcam.ino` file in Arduino IDE.
3. Select the correct board: **AI Thinker ESP32-CAM**.
4. Configure the correct COM port for your ESP32-CAM module.

### 3. Upload the Code

1. Connect the ESP32-CAM to your computer using a USB-to-Serial adapter.
2. Press the **BOOT** button on the ESP32-CAM while uploading the code.
3. Once uploaded, open the Serial Monitor (115200 baud rate) to view logs.

### 4. Run the System

- The system initializes the camera and starts detecting motion.
- Motion detection alerts and scores are displayed in the Serial Monitor.

## Code Overview

### Main Components

1. **`espcam.ino`**: Contains the main setup and loop functions.
2. **`cam.h`**: Header file defining camera-related functions.
3. **`cam.cpp`**: Implements camera initialization and motion detection logic.

### Key Functions

- `initCamera()`: Initializes the ESP32-CAM module.
- `detectMotion()`: Compares consecutive frames to detect motion.
- `getLastMotionScore()`: Retrieves the last motion detection score.

## Configuration

- **Motion Threshold**: Adjust the `MOTION_THRESHOLD` value in `cam.cpp` to change sensitivity.
- **Change Limit**: Modify the `CHANGE_LIMIT` value to control the motion detection trigger.

## Example Output

```plaintext
Camera initialized. Starting motion detection...
⚠️ Motion detected!
```

## Troubleshooting

- Ensure the ESP32-CAM is powered correctly.
- Check the Serial Monitor for error messages.
- Verify the camera connections and pins in `cam.cpp`.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- Inspired by the capabilities of the AI Thinker ESP32-CAM module.
- Special thanks to the open-source community for their contributions.

---

Feel free to contribute to this project by submitting issues or pull requests on [GitHub](https://github.com/LansariFedi/espcam-motion-detection).
