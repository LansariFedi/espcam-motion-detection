from flask import Flask, request

app = Flask(__name__)
@app.route("/")
def home():
    return '''
    <html>
        <body>
            <h1>ESP32-CAM Image Viewer</h1>
            <img src="/image" alt="ESP32-CAM Image" style="width:100%;max-width:600px;">
        </body>
    </html>
    '''

@app.route("/image", methods=["POST"])
def receive_image():
    image = request.files.get("image")
    if image:
        image.save("received_image.jpg")
        return "Image received and saved!", 200
    return "No image received!", 400

@app.route("/image", methods=["GET"])
def serve_image():
    try:
        with open("received_image.jpg", "rb") as img_file:
            return img_file.read(), 200, {"Content-Type": "image/jpeg"}
    except FileNotFoundError:
        return "No image available!", 404

# Ensure the app runs when the script is executed directly
if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)