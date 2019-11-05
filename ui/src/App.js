import React, {useState, useEffect} from 'react';
import map from './maps/risk_pic.png'
import './App.css';
import data_test from './data/state.xml'
import convert from 'xml-js'



const App = () => {

  const [data, setData] = useState([])

  useEffect(() => {
    setInterval(() => readFile(), 1000);
  }, []);

    const readFile = () => {
        fetch(data_test)
        .then((r) => r.text())
        .then(text  => {
         setData(convert.xml2js(text, {compact: true}).map.country)
        })
    }


  return (
    <div className="App">
      <div style={{position:"relative", width:"100%"}} >
        <img src={map}/> 
        {console.log(data)}
        {data.map((element, index) =>{ 
          console.log(element)
          return <div key={index} style={{
            position:"absolute",
            left:Number(element.position.x._text),
            top:Number(element.position.y._text)-10,
            backgroundColor: element.player_color._text,
            borderRadius:50,
            height: 25,
            width: 25,
            display: "inline-block",
            textAlign: "center",
            color: (element.player_color._text == "Black" ? "white" : "black" )
          }}> 
              {element.army._text} 
            </div> 
        })} 
      </div>
    </div>
  );
}

export default App;
