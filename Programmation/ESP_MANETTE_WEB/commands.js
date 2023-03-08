function setupWebsocket() {
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
        switch(jsonObject.type) {
          case "info":    
          case "reseau":  
          case "ssid":                       
          case "chip": document.getElementById("output").innerHTML= evt.data;break;
          case "udp": document.getElementById("output").innerHTML= jsonObject.Port;break;                            
        }   
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
    }
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
  function updateDisplay(){
    count++;
    counterDisplayElem.innerHTML = count;
  };
  function containsOnlyNumbers(str) {
    return /^[0-9]+$/.test(str);
  }
};


