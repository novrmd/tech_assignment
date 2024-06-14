# Import necessary libraries
from flask import Flask, request, jsonify
import pandas as pd
from datetime import datetime
import pytz

# Initialize Flask app
app = Flask(__name__)

# Data storage
sensor_data = {
    "timestamp": [],
    "temperature": [],
    "humidity": []
}

@app.route('/')
def home():
    # Convert the sensor data dictionary to a DataFrame
    data_frame = pd.DataFrame.from_dict(sensor_data)
    # Return the DataFrame as an HTML table
    return data_frame.to_html(), 200

@app.route('/get', methods=['GET'])
def get_data():
    # Get the current time in the Asia/Jakarta timezone
    current_time = datetime.now(pytz.timezone('Asia/Jakarta')).strftime("%d/%m/%Y %H:%M:%S")
    # Get temperature and humidity from the GET request parameters
    temp = float(request.args.get("temperature"))
    hum = float(request.args.get("humidity"))

    # Append the data to the sensor_data dictionary
    sensor_data['timestamp'].append(current_time)
    sensor_data['temperature'].append(temp)
    sensor_data['humidity'].append(hum)

    # Return the data as a JSON response
    return jsonify({
        'timestamp': current_time,
        'temperature': temp,
        'humidity': hum
    })

@app.route('/post', methods=['POST'])
def post_data():
    # Get the current time in the Asia/Jakarta timezone
    current_time = datetime.now(pytz.timezone('Asia/Jakarta')).strftime("%d/%m/%Y %H:%M:%S")
    # Get the JSON data from the POST request
    data = request.get_json()
    temp = float(data['temperature'])
    hum = float(data['humidity'])

    # Append the data to the sensor_data dictionary
    sensor_data['timestamp'].append(current_time)
    sensor_data['temperature'].append(temp)
    sensor_data['humidity'].append(hum)

    # Return the data as a JSON response
    return jsonify({
        'timestamp': current_time,
        'temperature': temp,
        'humidity': hum
    })

if __name__ == "__main__":
    # Run the Flask app on all available IP addresses (0.0.0.0) and port 5000
    app.run(host='0.0.0.0', port=5000, debug=True)
