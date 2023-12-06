import React, { useState } from 'react';
import Slider from 'rc-slider';
import Modal from 'react-modal';
import 'rc-slider/assets/index.css';

const InputSection = () => {
    const [numObject, setNumberOfObjects] = useState(77);
    const [sizeMaze, setSizeMaze] = useState(2002);
    const [diffLevel, setDiffLevel] = useState(12);
    const [distance, setDistance] = useState(10);
    const [modalIsOpen, setModalIsOpen] = useState(false);
    const [currentImage, setCurrentImage] = useState('/HanoiCity.png');
    const useExData = true;

    const handleSliderNumberObjectChange = (numberObjectValue) => {
        setNumberOfObjects(numberObjectValue);
    };

    const handleSliderSizeMazeChange = (sizeMazeValue) => {
        setSizeMaze(sizeMazeValue);
    };

    const handleSliderDiffLevelChange = (diffLevelValue) => {
        setDiffLevel(diffLevelValue);
    };

    const handleSliderDistanceChange = (distanceValue) => {
        setDistance(distanceValue);
    };

    const handleCustomData = async () => {
        console.log('Number of objects:', numObject);
        console.log('Size of maze:', sizeMaze);
        console.log('Difficult level:', diffLevel);
        console.log('Distance start and end point', distance);
        // Export Data to Backend
        const dataToSend = {
            numObject,
            sizeMaze,
            diffLevel,
            distance,
        };

        try {
            const response = await fetch('http://localhost:8080/load-custom-data', {
                mode: 'no-cors',
                credentials: 'include',
                method: 'POST',
                body: JSON.stringify(dataToSend),
            });

            if (response.type === 'opaque') {
                console.log('Request load custom data may not have CORS support');
            } else {
                console.log('Send request with custom data successful');
            }
        } catch (error) {
            console.error('Error during export:', error);
        }
    };

    const handleExperimentData = async () => {
        // Export Data to Backend
        const dataToSend = {
            useExData
        };

        try {
            const response = await fetch('http://localhost:8080/load-experiment-data', {
                mode: 'no-cors',
                credentials: 'include',
                method: 'POST',
                body: JSON.stringify(dataToSend),
            });

            if (response.type === 'opaque') {
                console.log('Request load experiment data may not have CORS support');
            } else {
                console.log('Request successful');
            }
        } catch (error) {
            console.error('Error during export:', error);
        }
        console.log('Loaded experiment data to backend: RoadsResult.txt');
    };

    const handleDeepView = () => {
        if (currentImage === '/HanoiCity.png') {
            setCurrentImage('/RoadsResult.png');
        } else {
            setCurrentImage('/HanoiCity.png');
        }
    };

    const openModal = () => {
        setModalIsOpen(true);
    };

    const closeModal = () => {
        setModalIsOpen(false);
    };

    const buttonStyle = {
        backgroundColor: '#87CEEA', // Màu nền
        color: '#FAFAFA',             // Màu chữ
        padding: '10px 20px',        // Kích thước padding
        borderRadius: '5px',        // Bo góc
        border: 'none',             // Không có đường viền
        cursor: 'pointer',          // Hiển thị biểu tượng con trỏ khi di chuột vào
        fontSize: '16px',           // Kích thước chữ
        // width: '100%',
        marginRight: '10px', // Add space between buttons
        margin: '10px',
    };

    return (
        <div className="input-section">
            <div>
                <label>Number of objects: {numObject}</label>
                <Slider
                    min={3}
                    max={100}
                    value={numObject}
                    onChange={handleSliderNumberObjectChange}
                />
            </div>
            <div>
                <label>Size of maze: {sizeMaze}</label>
                <Slider
                    min={100}
                    max={15000}
                    value={sizeMaze}
                    onChange={handleSliderSizeMazeChange}
                />
            </div>
            <div>
                <label>Difficult level: {diffLevel}</label>
                <Slider
                    min={10}
                    max={25}
                    value={diffLevel}
                    onChange={handleSliderDiffLevelChange}
                />
            </div>
            <div>
                <label>Distance start and end point: {distance}</label>
                <Slider
                    min={1}
                    max={90}
                    value={distance}
                    onChange={handleSliderDistanceChange}
                />
            </div>
            <button onClick={handleCustomData} style={{
                backgroundColor: '#87CEEA', // Màu nền
                color: '#FAFAFA',             // Màu chữ
                padding: '10px',        // Kích thước padding
                borderRadius: '5px',        // Bo góc
                border: 'none',             // Không có đường viền
                cursor: 'pointer',          // Hiển thị biểu tượng con trỏ khi di chuột vào
                fontSize: '16px',           // Kích thước chữ
                width: '95%',
                marginRight: '10px', // Add space between buttons
                margin: '10px',
                alignItems: 'center',    
            }}>Export Custom Parameter</button>

            <hr style={{ border: '1px solid #ccc' }} />

            <div style={{ display: 'flex', height: '20vh' }} >
                <img
                    src="/HanoiCity.png"
                    alt="Thumbnail"
                    onClick={openModal}
                    style={{
                        cursor: 'pointer',
                        width: '70%',
                        height: 'auto',
                        flex: 2,
                        border: '1px solid #87CEEA',
                        margin: '2px',
                        padding: '2px',
                        borderRadius: 10
                    }}
                />
                <Modal
                    isOpen={modalIsOpen}
                    onRequestClose={closeModal}
                    contentLabel="Image Modal"
                    style={{
                        content: {
                            display: 'flex',
                            flex: 3,
                            border: '1px solid #ccc',
                            margin: '2px',
                            padding: '2px',
                            borderRadius: 10
                        },
                    }}
                >
                    <img
                        src={currentImage}
                        alt="Large"
                        style={{
                            cursor: 'pointer',
                            width: '70%',
                            height: 'auto',
                            flex: 2,
                            border: '1px solid #ccc',
                            margin: '40px',
                            padding: '2px',
                            borderRadius: 10
                        }}
                    />
                    <div style={{ margin: '10px', alignItems: 'center' }}>
                        <button onClick={handleDeepView} style={buttonStyle}>
                            Deep View
                        </button>
                        <button onClick={closeModal} style={buttonStyle}>
                            Close
                        </button>
                    </div>
                </Modal>

                <button onClick={handleExperimentData} style={buttonStyle}>
                    Export
                </button>

            </div>
        </div>
    );
}

export default InputSection;
