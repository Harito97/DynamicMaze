import React, { useState, useEffect } from 'react';

const ImageFrame = () => {
  // const [imageData, setImageData] = useState('');

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

  return (
    <div className="image-frame">
      Image Frames
      {/* {imageData && <img src={`data:image/png;base64,${imageData}`} alt="Sample Image" />} */}
    </div>
  );
}

export default ImageFrame;
