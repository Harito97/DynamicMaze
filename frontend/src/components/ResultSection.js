import React, { useState } from 'react';

const ResultSection = () => {
    const [consoleContent, setConsoleContent] = useState('');

    const appendToConsole = (message) => {
        setConsoleContent((prevContent) => prevContent + message + '\n');
    };

    // Example: Append some content to the console on component mount
    React.useEffect(() => {
        appendToConsole('Welcome to terminal!');
    }, []); // Empty dependency array ensures it runs only once on mount

    return (
        // <div className="result-section">
        <div className="console-window"
            style={{
                content: {
                    display: 'flex',
                    border: '3px solid #ccc',
                    margin: '2px',
                    padding: '2px',
                    borderRadius: 15
                },
            }}>
            <textarea
                value={consoleContent}
                readOnly
                rows={10}
                style={{
                    resize: 'none',
                    width: '92%',
                    fontFamily: 'monospace',
                    // backgroundColor: '#272822',
                    // color: '#F8F8F2',
                    border: 'none',
                    padding: '15px',
                    fontSize: '14px',
                    // boxShadow: '0px 0px 10px rgba(0, 0, 0, 0.1)',
                    outline: 'none',
                }}
            />
        </div>
        // </div>
    );
};

export default ResultSection;

