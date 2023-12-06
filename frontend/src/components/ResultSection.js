import React, { useState, useEffect } from 'react';

const ResultSection = () => {
    const [consoleContent, setConsoleContent] = useState('Welcome to terminal!\n');

    const appendToConsole = (message) => {
        setConsoleContent((prevContent) => prevContent + message + '\n');
    };

    useEffect(() => {
        const getResult = async () => {
            try {
                const response = await fetch('http://localhost:8080/get-txt-result', {
                    method: 'GET',
                    headers: {
                        'Content-Type': 'application/json',
                    },
                    mode: 'no-cors',
                    credentials: 'include',
                });

                const resultText = await response.text();                
                console.log('Request successful');
                appendToConsole(resultText);
            } catch (error) {
                console.error('Error during get result:', error);
            }
        };

        // Gọi hàm để lấy kết quả khi component được mount
        getResult();
    }, []); // Empty dependency array ensures it runs only once on mount

    return (
        <div className="console-window">
            <textarea
                value={consoleContent}
                readOnly
                rows={10}
                style={{
                    resize: 'none',
                    fontFamily: 'monospace',
                    border: 'none',
                    padding: '15px',
                    fontSize: '14px',
                    outline: 'none',
                }}
            />
        </div>
    );
};

export default ResultSection;
