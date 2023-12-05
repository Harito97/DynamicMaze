import cv2
import numpy as np

def extract_roads(image_path):
    # Read image from the specified path
    img = cv2.imread(image_path)

    # Check if the image is loaded successfully
    if img is None:
        print(f"Error: Unable to read the image from '{image_path}'. Make sure the file exists.")
        return

    # Convert the image to grayscale
    gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Apply image processing techniques to highlight roads
    blurred_img = cv2.GaussianBlur(gray_img, (5, 5), 0)
    edges = cv2.Canny(blurred_img, 50, 150)

    # Find contours of the roads
    contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # Create a black image with the same size as the original image
    road_mask = np.zeros_like(gray_img)

    # Draw contours of the roads on the black image
    cv2.drawContours(road_mask, contours, -1, (255), thickness=cv2.FILLED)

    # Dilate the white regions around contours
    kernel = np.ones((3, 3), np.uint8)
    road_mask = cv2.dilate(road_mask, kernel, iterations=1)

    # Save the resulting image in the same folder as the script
    result_path = 'frontend/public/RoadsResult.png'
    cv2.imwrite(result_path, road_mask)

    # Convert the boolean mask to binary (0 or 1)
    # binary_mask = (road_mask > 0).astype(int)
    binary_mask = (~road_mask.astype(bool)).astype(int)

    # Save the binary mask as a text file
    txt_path = 'frontend/public/RoadsResult.txt'
    np.savetxt(txt_path, binary_mask, fmt='%d', delimiter='')

def main():
    # Specify the image path
    image_path = 'frontend/public/HanoiCity.png'
    # Call the function with the image path
    extract_roads(image_path)

# main()