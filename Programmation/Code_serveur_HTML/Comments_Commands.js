function setupWebsocket() {
	// Définition des configurations par défaut pour les différents cas
	var confdefaut = {1:100,2:115,3:119,4:32,5:97,6:2,7:113,8:1,9:101,10:178,11:13};
	var confFraming = {1:100,2:115,3:119,4:118,5:97,6:113,7:113,8:104,9:179,10:1,11:2};
	var confBros = {1:100,2:115,3:119,4:1,5:97,6:2,7:4,8:2,9:101,10:215,11:216};
	var confCOD = {1:100,2:115,3:119,4:1,5:97,6:2,7:114,8:2,9:101,10:27,11:128};

	// Définition de l'URI du websocket et création de l'objet WebSocket
	var wsUri = "ws://" + location.host + "/";
	ws = new WebSocket(wsUri);

	// Actions à effectuer lorsque la connexion WebSocket est ouverte
	ws.onopen = function(evt) { 
	  console.log('websock open'); 
	  //ws.send("esp?\n");
	};

	// Actions à effectuer lorsque la connexion WebSocket est fermée
	ws.onclose = function(evt) {
	  document.getElementById("output").innerHTML="Socket non connecté";	
	  console.log('websock close'); 
	};

	// Actions à effectuer en cas d'erreur de connexion WebSocket
	ws.onerror = function(evt) { console.log(evt); };

	// Actions à effectuer lors de la réception d'un message par WebSocket
	ws.onmessage = function(evt) {
	  console.log("Message Rx: " + evt.data);
		
	  if (typeof evt.data === "string") {
		try {
		  // essai de créer un objet JSON 
		  var jsonObject = JSON.parse(evt.data);

		  // Mise à jour des éléments HTML avec les valeurs reçues dans le JSON
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

	// Récupération de l'élément HTML avec la classe "counter-display
	let counterDisplayElem = document.querySelector('.counter-display');
	let count = 0;

	  // Le code ci-dessous écoute l'événement keydown sur l'élément 'commande' et appelle la fonction 'keyGotPressed' lorsqu'il se produit.
	commande.addEventListener('keydown', keyGotPressed, false);

	function keyGotPressed (e){
	  var charCode = e.keyCode;
	  var valeur = document.getElementById('commande');
	  var bouton = document.getElementById('bouton');
	  var valueTouche = valeur.value;
	  var strValue = valueTouche.charCodeAt(0);
	  var valueBtn = bouton.value;
	  var strBtn = parseInt(valueBtn);
	  
	  // Création d'un objet avec les valeurs du bouton et de la touche pressée
	  const obj = {id:strBtn, valeur:strValue};
	  
	  // Conversion de l'objet en JSON
	  const myJSON = JSON.stringify(obj);
	  
	  // Si la touche pressée est "Entrée" et que la valeur est un nombre, on effectue les actions suivantes
	  if((charCode==13) && containsOnlyNumbers(strValue) == true){
		updateDisplay();
		console.log(myJSON);
		ws.send(myJSON+"\n");
		clearfct();
	  }
	}

	const save_element = document.getElementById("savebtn");
	save_element.addEventListener("click", save);

	function save (){
	  var valeur = document.getElementById('commande');
	  var bouton = document.getElementById('bouton');
	  var valueTouche = valeur.value;
	  var strValue = valueTouche.charCodeAt(0);
	  var valueBtn = bouton.value;
	  var strBtn = parseInt(valueBtn);
	  
	  // Création d'un objet avec les valeurs du bouton et de la touche pressée
	  const obj = {id:strBtn, valeur:strValue};
	  
	  // Conversion de l'objet en JSON
	  const myJSON = JSON.stringify(obj);
	  
	  // Si la valeur est un nombre, on effectue les actions suivantes
	  if(containsOnlyNumbers(strValue) == true){
		updateDisplay();
		console.log(myJSON);
		ws.send(myJSON+"\n");
		clearfct();
	  }
	}

	const back_element = document.getElementById("backbtn");
	back_element.addEventListener("click", clearfct);

	function clearfct (){
	  document.getElementById('commande').value = "";
	}

	const refresh_element = document.getElementById("refreshbtn");
	refresh_element.addEventListener("click", refreshfct);

	function refreshfct (){
	  // Création d'un objet avec des valeurs spécifiques
	  const obj = {id:999, valeur:999};
	  
	  // Conversion de l'objet en JSON
	  const myJSON = JSON.stringify(obj);
	  
	  // Appel de la fonction updateDisplay et envoi du JSON via WebSocket
	  updateDisplay();
	  console.log(myJSON);
	  ws.send(myJSON+"\n");
	};
	
	// Récupération de l'élément HTML avec l'id "special"
	var spe = document.getElementById("special");

	// Ajout d'un écouteur d'événement pour le changement de valeur de l'élément HTML récupéré
	spe.addEventListener("change", selectOption, false);

	// Fonction appelée lorsqu'une option est sélectionnée
	function selectOption (){

	  // Récupération des éléments HTML avec les id "special" et "bouton"
	  var valeur = document.getElementById('special');
	  var bouton = document.getElementById('bouton');

	  // Récupération de la valeur sélectionnée dans l'élément HTML "special"
	  var specialVal = valeur.value;

	  // Conversion de la valeur récupérée en entier
	  var strValue = parseInt(specialVal);

	  // Récupération de la valeur du bouton
	  var valueBtn = bouton.value;

	  // Conversion de la valeur du bouton en entier
	  var strBtn = parseInt(valueBtn);

	  // Création d'un objet avec les valeurs récupérées
	  const obj = {id:strBtn,valeur:strValue};

	  // Conversion de l'objet en chaîne de caractères JSON
	  const myJSON = JSON.stringify(obj);

	  // Appel de la fonction "updateDisplay" pour mettre à jour l'affichage
	  updateDisplay();

	  // Affichage de la chaîne de caractères JSON dans la console
	  console.log(myJSON);

	  // Envoi de la chaîne de caractères JSON via une connexion WebSocket
	  ws.send(myJSON+"\n");
	};

	// Récupération de l'élément HTML avec l'id "preset"
	var spec = document.getElementById("preset");

	// Ajout d'un écouteur d'événement pour le changement de valeur de l'élément HTML récupéré
	spec.addEventListener("change", selectPreset, false);

	// Fonction appelée lorsqu'une option est sélectionnée dans l'élément HTML "preset"
	function selectPreset (){

	  // Récupération de l'élément HTML avec l'id "preset"
	  var valeur = document.getElementById('preset');

	  // Récupération de la valeur sélectionnée dans l'élément HTML "preset"
	  var specialVal = valeur.value;

	  // Définition de la variable "strValue" en fonction de la valeur sélectionnée dans l'élément HTML "preset"
	  if(specialVal == 1000)
		var strValue = confdefaut;
	  if(specialVal == 1001)
		var strValue = confFraming;
	  if(specialVal == 1002)
		var strValue = confBros;
	  if(specialVal == 1003)
		var strValue = confCOD;

	  // Conversion de la valeur en chaîne de caractères JSON
	  const myJSON = JSON.stringify(strValue);

	  // Appel de la fonction "updateDisplay" pour mettre à jour l'affichage
	  updateDisplay();

	  // Affichage de la chaîne de caractères JSON dans la console
	  console.log(myJSON);

	  // Envoi de la chaîne de caractères JSON via une connexion WebSocket
	  ws.send(myJSON+"\n");
	};
	
	// Fonction pour mettre à jour l'affichage
	function updateDisplay(){
	  count++; // Incrémente la variable count de 1
	  counterDisplayElem.innerHTML = count; // Met à jour l'élément HTML avec la nouvelle valeur de count
	};

	// Fonction pour vérifier si une chaîne de caractères contient uniquement des chiffres
	function containsOnlyNumbers(str) {
	  return /^[0-9]+$/.test(str); // Retourne true si la chaîne ne contient que des chiffres, false sinon
	}
};
