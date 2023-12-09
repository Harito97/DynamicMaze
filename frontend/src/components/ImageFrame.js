import React, { useState, useEffect } from 'react';

const ImageFrame = () => {
  const [imageData, setImageData] = useState('');
  // useEffect(() => {
  //   // Gửi yêu cầu GET để nhận ảnh từ backend khi component mount
  //   fetch('http://localhost:8080/get-img-result', {
  //     mode: 'no-cors',
  //     credentials: 'include',
  //     method: 'POST',
  //     headers: {
  //       'Content-Type': 'application/json',
  //     }
  //   })
  //     .then(response => response.text())
  //     .then(data => {
  //       // Set ảnh nhận được vào state để hiển thị
  //       setImageData(data);
  //     })
  //     .catch(error => console.error('Error fetching image:', error));
  // }, []); // Empty dependency array ensures it runs only once on mount

  const handleExportPicture = async () => {
    try {
      const response = await fetch('http://localhost:8080/get-img-result', {
        mode: 'cors',
        // // origin: 'http://localhost:3000',
        method: 'GET',
      });

      const imageDataResult = await response.text();
      console.log('Request successful', imageDataResult);
      setImageData(imageDataResult);
    } catch (error) {
      console.error('Error during get result:', error);
    }
  };

  const buttonStyle = {
    backgroundColor: '#87CEEA', // Màu nền
    color: '#FAFAFA',             // Màu chữ
    padding: '10px 20px',        // Kích thước padding
    borderRadius: '5px',        // Bo góc
    border: 'none',             // Không có đường viền
    cursor: 'pointer',          // Hiển thị biểu tượng con trỏ khi di chuột vào
    fontSize: '16px',           // Kích thước chữ
    width: '98%',
    marginRight: '10px', // Add space between buttons
    margin: '10px',
  };

  return (
    <div className="image-frame">
      <div>
        {/* Image Frames */}
        {imageData && <img src={`data:image/png;base64,${imageData}`} alt="Sample Image" style={{ width: '98.5%', padding: '5px', border: '1px solid #ccc', borderRadius: '15px', alignItems: 'center', justifyContent: 'center', boxShadow: '0px 0px 10px rgba(0, 0, 0, 0.5)', }} onMouseOver={handleExportPicture} />}
      </div>
      <button onMouseOver={handleExportPicture} style={buttonStyle}>
        Move the mouse in this button Or in the picture
      </button>
    </div>
  );
}

export default ImageFrame;
