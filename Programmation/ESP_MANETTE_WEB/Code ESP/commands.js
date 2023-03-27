function setupWebsocket() {

  var confdefaut = {1:100,2:115,3:119,4:32,5:97,6:2,7:113,8:1,9:101,10:178,11:13};
  var confFraming = {1:100,2:115,3:119,4:118,5:97,6:113,7:113,8:104,9:179,10:1,11:2};
  var confBros = {1:100,2:115,3:119,4:1,5:97,6:2,7:4,8:2,9:101,10:215,11:216};
  var confCOD = {1:100,2:115,3:119,4:1,5:97,6:2,7:114,8:2,9:101,10:27,11:128};

  var wsUri = "ws://" + location.host + "/";
  ws = new WebSocket(wsUri);
  ws.onopen = function(evt) { 
    console.log('websock open'); 
    //ws.send("esp?\n");
  };
  ws.onclose = function(evt) {
    document.getElementById("output").innerHTML="Socket non connecté";	
    console.log('websock close'); 
  };
  ws.onerror = function(evt) { console.log(evt); };
  ws.onmessage = function(evt) {
    console.log("Message Rx: " + evt.data);
		
	if (typeof evt.data === "string") {
      try {
        // essai de créer un objet JSON 
        var jsonObject = JSON.parse(evt.data);
        document.getElementById("bleu1").innerHTML = "Bleu : " + jsonObject.b1;
        document.getElementById("rouge1").innerHTML = "Rouge : " + jsonObject.b2;
        document.getElementById("vert1").innerHTML = "Vert :  : " + jsonObject.b3;
        document.getElementById("blanc1").innerHTML = "Blanc : " + jsonObject.b4;
        document.getElementById("jaune1").innerHTML = "Jaune : " + jsonObject.b5;

        document.getElementById("bleu2").innerHTML = "Bleu : " + jsonObject.b9;
        document.getElementById("rouge2").innerHTML = "Rouge : " + jsonObject.b6;
        document.getElementById("vert2").innerHTML = "Vert : " + jsonObject.b8;
        document.getElementById("jaune2").innerHTML = "Jaune : " + jsonObject.b7;

        document.getElementById("aux1").innerHTML = "Aux. 1 : " + jsonObject.b10;
        document.getElementById("aux2").innerHTML = "Aux. 2 : " + jsonObject.b11;
      }
      catch( e ) {
        // pas du json  
        // affiche texte dans la zone output
        var ligne = evt.data;
        ligne = ligne.replace(/\n/g,"<br>")
        document.getElementById("output").innerHTML= ligne;
      }		
	  }    	
  };	
  let counterDisplayElem = document.querySelector('.counter-display');
  let count = 0;

  commande.addEventListener('keydown', keyGotPressed, false);
  function keyGotPressed (e){
    var charCode = e.keyCode;
    var valeur = document.getElementById('commande');
    var bouton = document.getElementById('bouton');
    var valueTouche = valeur.value;
    var strValue = valueTouche.charCodeAt(0);
    var valueBtn = bouton.value;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    if((charCode==13) && containsOnlyNumbers(strValue) == true){
        updateDisplay();
        console.log(myJSON);
        ws.send(myJSON+"\n");
        clearfct();
    }
  };

  const save_element = document.getElementById("savebtn");
  save_element.addEventListener("click", save);
  function save (){
    var valeur = document.getElementById('commande');
    var bouton = document.getElementById('bouton');
    var valueTouche = valeur.value;
    var strValue = valueTouche.charCodeAt(0);
    var valueBtn = bouton.value;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    if(containsOnlyNumbers(strValue) == true){
        updateDisplay();
        console.log(myJSON);
        ws.send(myJSON+"\n");
        clearfct();
    }
  };

  const back_element = document.getElementById("backbtn");
  back_element.addEventListener("click", clearfct);
  function clearfct (){
    document.getElementById('commande').value = "";
  };

  const refresh_element = document.getElementById("refreshbtn");
  refresh_element.addEventListener("click", refreshfct);
  function refreshfct (){
    const obj = {id:999,valeur:999};
    const myJSON = JSON.stringify(obj);
    updateDisplay();
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("special");
  spe.addEventListener("change", selectOption, false);
  function selectOption (){
    var valeur = document.getElementById('special');
    var bouton = document.getElementById('bouton');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = bouton.value;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    updateDisplay();
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spec = document.getElementById("preset");
  spec.addEventListener("change", selectPreset, false);
  function selectPreset (){
    var valeur = document.getElementById('preset');
    var specialVal = valeur.value;
    if(specialVal == 1000)
      var strValue = confdefaut;
    if(specialVal == 1001)
      var strValue = confFraming;
    if(specialVal == 1002)
      var strValue = confBros;
    if(specialVal == 1003)
      var strValue = confCOD;
    const myJSON = JSON.stringify(strValue);
    updateDisplay();
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  function updateDisplay(){
    count++;
    counterDisplayElem.innerHTML = count;
  };

  function containsOnlyNumbers(str) {
    return /^[0-9]+$/.test(str);
  }
};


