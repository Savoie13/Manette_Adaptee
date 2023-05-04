/*
Nom du fichier : commands.js
Auteur : Jackob Breton 
Date de création : 2023-02-06
Description : La fonction setupWebsocket() initialise une connexion WebSocket vers un serveur distant
 et définit plusieurs configurations de communication. Ensuite, elle définit plusieurs fonctions pour
 traiter les événements qui se produisent sur la connexion WebSocket, tels que l'ouverture, la
 fermeture, l'erreur et la réception de messages.

 Lorsqu'un message est reçu, la fonction onmessage essaie de créer un objet JSON à partir de la chaîne
 de caractères reçue. Si cela réussit, elle met à jour les valeurs des boutons dans l'interface utilisateur
 en utilisant les données du JSON. Sinon, elle affiche simplement le texte brut dans la zone de sortie.

 De plus, la fonction setupWebsocket() ajoute également un gestionnaire d'événements de clic sur un bouton
 nommé refreshbtn. Lorsque ce bouton est cliqué, un objet JSON est créé et envoyé sur la connexion WebSocket.

 Enfin, la fonction définit une série de fonctions qui sont appelées lorsqu'un élément spécifique de
 l'interface utilisateur change, comme lorsqu'un utilisateur sélectionne une nouvelle valeur pour un menu
 déroulant. Ces fonctions créent un objet JSON contenant les données pertinentes et les envoient via la
 connexion WebSocket au serveur distant.
*/
function setupWebsocket() {

  var confdefaut = {1:100,2:115,3:119,4:32,5:97,6:2,7:113,8:1,9:101,10:178,11:13};
  var confFraming = {1:100,2:115,3:119,4:118,5:97,6:113,7:113,8:104,9:179,10:1,11:2};
  var confBros = {1:100,2:115,3:119,4:1,5:97,6:2,7:4,8:2,9:101,10:215,11:216};
  var confCOD = {1:100,2:115,3:119,4:1,5:97,6:2,7:114,8:2,9:101,10:27,11:128};

  var wsUri = "ws://" + location.host + "/ws";
  ws = new WebSocket(wsUri);
  ws.onopen = function(evt) { // si 
    console.log('websock open'); 
    //ws.send("esp?\n");
  };
  ws.onclose = function(evt) { // si le websocket est fermé...
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
		// affiche la valeur du bouton selon la valeur du JSON
        document.getElementById("menuBleuD").value = jsonObject.b1;
        document.getElementById("menuRougeD").value = jsonObject.b2;
        document.getElementById("menuVertD").value = jsonObject.b3;
        document.getElementById("menuBlancD").value = jsonObject.b4;
        document.getElementById("menuJauneD").value = jsonObject.b5;

        document.getElementById("menuBleuC").value = jsonObject.b9;
        document.getElementById("menuRougeC").value = jsonObject.b6;
        document.getElementById("menuVertC").value = jsonObject.b8;
        document.getElementById("menuJauneC").value = jsonObject.b7;

        document.getElementById("menuAux1").value = jsonObject.b10;
        document.getElementById("menuAux2").value = jsonObject.b11;
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

  // permet d'envoyer une requête à l'arduino Pro Micro pour afficher la liste des boutons sur le site web.
  const refresh_element = document.getElementById("refreshbtn"); // l'élément du bouton est dans cette variable.
  refresh_element.addEventListener("click", refreshfct); // si le bouton est appuyé...
  function refreshfct (){
    const obj = {id:999,valeur:999}; // met cette valeur dans l'obj JSON
    const myJSON = JSON.stringify(obj); // tranforme l'objet en objet JSON
    console.log(myJSON); // affiche le message
    ws.send(myJSON+"\n"); // envoie le message
  };

  // Pour les 11 fonctions suivante, les commantaires sont les mêmes. C'est la même fonction
  // pour chacunes des différentes "dropbars". 
  
  var spe = document.getElementById("menuBleuD"); // Obtient l'élément du menu bleu avec l'id "menuBleuD"
  spe.addEventListener("change", selectBleuD, false); // Ajoute un événement de changement à l'élément "menuBleuD" qui appelle la fonction selectBleuD
  // La fonction selectBleuD est appelée lorsqu'un changement est apporté à l'élément "menuBleuD"
  function selectBleuD (){
	// Obtient la valeur de l'élément "menuBleuD"
    var valeur = document.getElementById('menuBleuD');
	// Convertit la valeur en un entier à l'aide de parseInt
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
	// Définit la valeur du bouton à 1 (pour bouton bleu) et la convertit également en entier
    var valueBtn = 1;
    var strBtn = parseInt(valueBtn);
	
    const obj = {id:strBtn,valeur:strValue};    // Crée un objet qui contient les valeurs de l'ID du bouton et de la valeur sélectionnée
    const myJSON = JSON.stringify(obj);     // Convertit l'objet en JSON
    // Envoie la chaîne JSON au serveur en utilisant WebSockets
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuRougeD");
  spe.addEventListener("change", selectRougeD, false);
  function selectRougeD (){
    var valeur = document.getElementById('menuRougeD');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 2;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuVertD");
  spe.addEventListener("change", selectVertD, false);
  function selectVertD (){
    var valeur = document.getElementById('menuVertD');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 3;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuBlancD");
  spe.addEventListener("change", selectBlancD, false);
  function selectBlancD (){
    var valeur = document.getElementById('menuBlancD');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 4;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuJauneD");
  spe.addEventListener("change", selectJauneD, false);
  function selectJauneD (){
    var valeur = document.getElementById('menuJauneD');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 5;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuBleuC");
  spe.addEventListener("change", selectBleuC, false);
  function selectBleuC (){
    var valeur = document.getElementById('menuBleuC');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 9;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuRougeC");
  spe.addEventListener("change", selectRougeC, false);
  function selectRougeC (){
    var valeur = document.getElementById('menuRougeC');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 6;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuVertC");
  spe.addEventListener("change", selectVertC, false);
  function selectVertC (){
    var valeur = document.getElementById('menuVertC');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 8;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuJauneC");
  spe.addEventListener("change", selectJauneC, false);
  function selectJauneC (){
    var valeur = document.getElementById('menuJauneC');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 7;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuAux1");
  spe.addEventListener("change", selectAux1, false);
  function selectAux1 (){
    var valeur = document.getElementById('menuAux1');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 10;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };

  var spe = document.getElementById("menuAux2");
  spe.addEventListener("change", selectAux2, false);
  function selectAux2 (){
    var valeur = document.getElementById('menuAux2');
    var specialVal = valeur.value;
    var strValue = parseInt(specialVal);
    var valueBtn = 11;
    var strBtn = parseInt(valueBtn);
    const obj = {id:strBtn,valeur:strValue};
    const myJSON = JSON.stringify(obj);
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
    console.log(myJSON);
    ws.send(myJSON+"\n");
  };
};


