from collections import deque
from ultralytics import YOLO
import time
import serial.tools.list_ports

# Load a model
model = YOLO("Classify_Wastes.pt")

# Parameters
stable_threshold = 5  # Number of consecutive frames to be stored in the queue
detection_queue = deque(maxlen=stable_threshold)
typeOfWaste = None

# Assuming these are your class names in the same order as your model's output
class_names = ["metal", "plastic", "other"]  # Modify this list as per your classes

# Function to send the result over the serial port
def send_serial_result(command, serialInst):
    if serialInst.is_open:
        serialInst.write(command.encode('utf-8'))
        print(f"Sent command {command} to {serialInst.port}")
    else:
        print(f"Serial port {serialInst.port} is not open.")

# Initialize serial connection
def initialize_serial_connection():
    ports = serial.tools.list_ports.comports()
    serialInst = serial.Serial()

    portsList = [str(onePort) for onePort in ports]
    for onePort in portsList:
        print(onePort)

    val = input("Select Port (just number after 'COM'): ")
    portVar = "COM" + str(val)
    if portVar not in portsList:
        print(f"Port {portVar} not found. Please check and try again.")
        return None

    serialInst.baudrate = 115200
    serialInst.port = portVar
    try:
        serialInst.open()
    except Exception as e:
        print(f"Failed to open serial port {portVar}: {e}")
        return None

    return serialInst

# Start serial connection
serialInst = initialize_serial_connection()
if serialInst is None:
    exit()

# Run inference on the camera feed
for result in model.predict(source=0, stream=True):
    detected_waste = None

    # Extract the class name of the detected object
    if result.boxes:
        class_ids = [int(box.cls.item()) for box in result.boxes]  # Convert class IDs to integers
        detected_wastes = [class_names[class_id] for class_id in class_ids]  # Map class IDs to class names
    else:
        detected_wastes = ["no detection"]

    # Add the detections to the queue
    detection_queue.extend(detected_wastes)
    print(f"Current detections: {detected_wastes}")
    print(f"Detection queue: {list(detection_queue)}")

    # Count occurrences of each waste type in the detection queue
    waste_counts = {waste: detection_queue.count(waste) for waste in class_names}
    print(f"Waste counts: {waste_counts}")

    # Check if any waste type has been detected more than 3 times
    for waste, count in waste_counts.items():
        if count > 3:
            final_prediction = waste
            if final_prediction != typeOfWaste:
                typeOfWaste = final_prediction
                if typeOfWaste == 'plastic':
                    command = '0'
                elif typeOfWaste == 'metal':
                    command = '1'
                elif typeOfWaste == 'other':
                    command = '2'

                send_serial_result(command, serialInst)
                print(f"Detected stable waste type: {typeOfWaste}")
            break  # Exit the loop once the command is sent

    # Optionally, display the results
    plot = result.plot()
    print("Plotting results...")

    # Add a delay if necessary to control the frame rate
    time.sleep(0.1)  # Adjust the delay as needed

# Close the serial connection
serialInst.close()
