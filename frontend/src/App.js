import React from 'react';
import './App.css';
import ImageFrame from './components/ImageFrame';
import InputSection from './components/InputSection';
import ResultSection from './components/ResultSection';

function App() {
  return (
    <div className="App">
      <ImageFrame />
      <div className="side-panel">
        <InputSection />
        <ResultSection />
      </div>
    </div>
  );
}

export default App;
