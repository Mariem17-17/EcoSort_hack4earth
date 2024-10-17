Here’s the corrected version with your changes:

---

# Smart Bin Network for Sustainable Urban Development

## Overview
This project focuses on a network of interconnected smart bins designed to optimize urban waste collection using IoT and AI technologies. The system uses a YOLOv8 custom model to classify waste items in real-time and control which bin compartment opens. The data is transmitted via LoRa from the nodes (bins) to the gateway and via Wi-Fi from the gateway to Firebase. A web-based dashboard provides insights into the fullness of all bins.

### Key Technologies:
- **YOLOv8**: Custom object detection model for classifying waste items (plastic, metal, paper).
- **ESP32**: Microcontroller handling bin compartment decisions based on classification.
- **LoRa**: Long-range communication for data transmission between bins and the gateway.
- **Wi-Fi**: Gateway communication with Firebase for real-time data storage.
- **Firebase**: Cloud platform for storing bin data and providing real-time updates.
- **Web-based Dashboard**: Displays bin status and insights on fullness levels for optimized waste management.

---

## Workflow

### 1. Waste Detection and Classification
- A YOLOv8 model, trained on a custom dataset, detects waste items placed in front of a camera.
- The model classifies the item into one of three categories (plastic, metal, paper) and forwards the information to the ESP32 microcontroller via serual communication.
- The ESP32 then controls which bin compartment opens to sort the waste accordingly.

### 2. Data Transmission and Storage
- Each smart node bin monitors the fullness of its compartments and transmits this data to the gateway via LoRa.
- The gateway sends all collected data to Firebase via Wi-Fi, enabling real-time monitoring on a user friendly dashboard.

### 3. Web-based Dashboard
- A custom-built website provides real-time insights into the status of all bins, including compartment fullness levels and location. This helps waste management teams make informed decisions and optimize collection routes.

---

## Installation and Setup

### Hardware Setup:
- **ESP32 Microcontroller**: Manages the waste sorting process and communicates with the gateway.
- **Camera**: Captures images of waste items for classification by the YOLOv8 model.
- **LoRa Module**: Facilitates long-range communication between bins and the gateway.
- **Gateway**: An ESP32 Microcontroller that connects to Wi-Fi and transmits data from all bins to Firebase.

### Software Requirements:
- **Python 3.x**: Used for the YOLOv8 model and data processing.
- **YOLOv8**: Custom-trained model for waste classification.
- **Firebase Account**: Stores data on bin statuses and provides cloud-based analytics.
- **Web Development Tools**: For building and deploying the real-time dashboard.

### Steps:
1. Set up the ESP32 with the camera for waste item detection.
2. Train and deploy the YOLOv8 model to classify waste items.
3. Configure the LoRa communication between bins and the gateway.
4. Set up the gateway to transmit data to Firebase via Wi-Fi.
5. Deploy the web-based dashboard for real-time monitoring of bin statuses.

---

## Usage

### Waste Sorting:
1. Place a waste item in front of the camera. 
2. The YOLOv8 model classifies the item and sends the result to the ESP32, which opens the corresponding bin compartment (plastic, metal, paper).
3. The bin also tracks the fullness of each compartment and sends this data to the gateway via LoRa.

### Monitoring:
- The gateway transmits bin data to Firebase via Wi-Fi, and the web-based dashboard provides real-time updates on bin fullness levels, helping optimize collection schedules.

---

## Future Improvements
- **AI Optimization**: Integrate predictive models to optimize waste collection routes further.
- **Cloud-based Analytics**: Enhance the dashboard with more advanced reporting and data analytics.
- **Scalability**: Expand the system to cover larger areas with more bins and gateways.

---

[![Watch the video](https://i.sstatic.net/Vp2cE.png)](https://youtu.be/vt5fpE0bzSY)
