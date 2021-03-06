function getScore()
{
	var response;
	var req = new XMLHttpRequest();
	
	req.open('GET', "http://twolinepass.ca/inc/scrape/getLiveScore.php?team=TOR", true);
	
	req.onload = function(e) 
	{
		if (req.readyState == 4) 
		{
			if(req.status == 200) 
			{
				//console.log(req.responseText);
				response = JSON.parse(req.responseText);
				var score;
				
				if (response.score) 
				{
					// data found, look for LastPrice
					score = response.score
					//console.log("score = "+score);
					Pebble.sendAppMessage({"0": "" + score.toString()});
				}
			} 
			else 
			{
				console.log("Request returned error code " + req.status.toString());
			}
		}
	}
	req.send(null);
};
				
Pebble.addEventListener("ready", function(e) 
{
	//getScore();
});

Pebble.addEventListener("appmessage", function(e)
{
	getScore();
});