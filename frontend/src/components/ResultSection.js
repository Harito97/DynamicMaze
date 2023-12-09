import React, { useState, useEffect } from 'react';

const ResultSection = () => {
    const [consoleContent, setConsoleContent] = useState('Welcome to terminal!\n');

    // const appendToConsole = (message) => {
    //     setConsoleContent((prevContent) => prevContent + message);
    // };

    const getResult = async () => {
        try {
            const response = await fetch('http://localhost:8080/get-txt-result', {
                mode: 'cors',
                // // origin: 'http://localhost:3000',
                method: 'GET',
            });

            const resultText = await response.text();
            console.log('Request successful', resultText);
            setConsoleContent(resultText);
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
        width: '100%'
    };

    return (
        <div className="console-window"
            style={{
                display: 'flex',
                flexDirection: 'column',
                alignItems: 'center',    // Để căn giữa theo chiều ngang
                // justifyContent: 'center', // Để căn giữa theo chiều dọc
                height: '100vh', // Để fill toàn bộ chiều cao của viewport
                padding: '10px'
            }}>
            <button onClick={getResult} style={buttonStyle}>
                Get Result
            </button>
            <textarea
                value={consoleContent}
                readOnly
                rows={10}
                style={{
                    resize: 'none',
                    border: 'none',
                    padding: '15px',
                    fontSize: '14px',
                    outline: 'none',
                    width: '90%'
                }}
            />
        </div>
    );
};

export default ResultSection;
