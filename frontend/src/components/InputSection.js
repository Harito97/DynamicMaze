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
            const response = await fetch('http://your-backend-api/export-custom-data', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(dataToSend),
            });

            if (response.ok) {
                console.log('Custom data exported successfully');
            } else {
                console.error('Failed to export custom data');
            }
        } catch (error) {
            console.error('Error during export:', error);
        }
    };

    const handleExperimentData = async () => {
        console.log('Loading experiment data to backend: RoadsResult.txt');
        // Export Data to Backend
        try {
            const response = await fetch('http://your-backend-api/load-experiment-data', {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json',
                },
            });

            if (response.ok) {
                const responseData = await response.json();
                console.log('Experiment data loaded successfully:', responseData);
                // Perform any additional frontend logic based on the loaded data
            } else {
                console.error('Failed to load experiment data');
            }
        } catch (error) {
            console.error('Error during experiment data loading:', error);
        }
    };

    const handleDeepView = () => {
        // Check the current image and toggle to the other image
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
                    max={10000}
                    value={sizeMaze}
                    onChange={handleSliderSizeMazeChange}
                />
            </div>
            <div>
                <label>Difficult level: {diffLevel}</label>
                <Slider
                    min={1}
                    max={40}
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
            <button onClick={handleCustomData}>Export</button>

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
                        border: '1px solid #ccc',
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
                    <div style={{ margin: '10px' }}>
                        <button
                            style={{
                                padding: '10px',
                                margin: '10px',
                                cursor: 'pointer',
                            }}
                            onClick={handleDeepView}
                        >
                            Deep View
                        </button>
                        <button
                            style={{
                                marginRight: '10px', // Add space between buttons
                                padding: '10px',
                                margin: '10px',
                                cursor: 'pointer',
                            }}
                            onClick={closeModal}
                        >
                            Close
                        </button>
                    </div>
                </Modal>

                <button
                    style={{
                        padding: '5px',
                        margin: '5px',
                        cursor: 'pointer',
                    }}
                    onClick={handleExperimentData}
                >
                    Export
                </button>

            </div>
        </div>
    );
}

export default InputSection;
