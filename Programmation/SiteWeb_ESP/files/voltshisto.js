// Initialize WebSocket connection and event handlers
function setupWebsocket() {
	var ctx = document.getElementById('canvas').getContext('2d');
	var wsUri = "ws://" + location.host + "/"; // pour ESP8266
	ws = new WebSocket(wsUri);

	var qtePoints =100; // qte de points du graphe à l"écran
		
	ws.onopen = function(e) {
		console.log("Client en ligne");
		// requête pour dernières valeurs
		//ws.send("adc histo\n");
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
					var pot = adc*0.09765625;
					var temps = jsonObject.ts; 
					if(temps == "")temps = new Date();
							
					for(i=0;i<lineChartData.datasets.length;i++){
						if(lineChartData.datasets[i].data.length >=qtePoints){
							lineChartData.datasets[i].data.shift();
						}
					}
					lineChartData.datasets[0].data.push({x:temps,y:pot.toFixed(0)});	
					window.myLine.update();
					
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



