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

    const handleChange = () => {
        console.log('Number of objects:', numObject);
        console.log('Size of maze:', sizeMaze);
        console.log('Difficult level:', diffLevel);
        console.log('Distance start and end point', distance);
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
            <button onClick={handleChange}>Call API</button>

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
                            // justifyContent: 'justify', // Center vertically
                            flex: 3,
                            border: '1px solid #ccc',
                            margin: '2px',
                            padding: '2px',
                            borderRadius: 10
                        },
                    }}
                >
                    <img
                        src="/HanoiCity.png"
                        alt="Large Image"
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
                            onClick={() => console.log('Clicked on the image')}
                        >
                            Export
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
                    onClick={() => console.log('Clicked on the image')}
                >
                    Export
                </button>

            </div>

        </div>
    );
}

export default InputSection;

