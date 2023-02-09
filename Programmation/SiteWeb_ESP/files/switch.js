// Initialize WebSocket connection and event handlers
function setupWebsocket() {
	var ctx = document.getElementById('jauge-volts').getContext('2d');
	var wsUri = "ws://" + location.host + "/"; // pour ESP8266
	ws = new WebSocket(wsUri);

	var qtePoints =150; // qte de points du graphe à l"écran
	var vmax = 3.3;		// valeur de la tension max au ADC, selon le diviseur de tension 
	var pot_old=0;
		
	ws.onopen = function(e) {
		console.log("Client en ligne");
		// requête pour dernières valeurs
		//ws.send("adc json\n");
	}

	// Listen for the close connection event
	ws.onclose = function(e) {
		log("WS hors ligne : " + e.reason);
	}

	// Listen for connection errors
	ws.onerror = function(e) {
		log("Erreur ");
	}

	// Listen for new messages arriving at the client
	ws.onmessage = function(e) {
		log("Message: " + e.data);
		
		if (typeof e.data === "string") {

			try {
				// Create a JSON object.
				var jsonObject = JSON.parse(e.data);
			
				if(jsonObject.type=="jmWiFi-POT"){
					
					// Extraire les valeurs des clés
					var adc = jsonObject.adc;
					//var pot = adc*100/1024;
					var potid = jsonObject.id;
					var SW = "OFF";
					//var volts = adc*vmax/1024;
					var ts = jsonObject.ts; 
					if(ts=="" || ts == undefined)ts=new Date();
					document.getElementById("time").innerHTML = ts;
					document.getElementById("potid").innerHTML=potid;
					var gaugeT = document.gauges.get('jauge-volts');
					//document.getElementById("volts").innerHTML=volts.toFixed(1) +' Volts';
					if(adc > 200){
						SW = "ON";
						gaugeT.options.colorNeedleCircleInner="#afa"
						gaugeT.options.colorNeedleCircleInnerEnd="#afa"

						gaugeT.options.colorNeedleCircleDuter="#aaa"
						gaugeT.options.colorNeedleCircleDuterEnd="#000"
					}
				}
			} catch (error) {
				log("data pas JSON");			
			}

		}
	}
}

// Display logging information in the document.
function log(s) {
	console.log(s);
}



