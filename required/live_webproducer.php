<?php
  /** 
    * @file
	* It's is a page external for view Live Producer.
	*
	*/
?>

  <?php
  
  function wimtvproWebProducer() {
  

	drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/swfObject/swfobject.js');
	drupal_add_css("
		#header{display:none;}
		 #footer-wrapper{display:none;}
		 #breadcrumb{display:none;}
		 .sidebar{display:none!important}"
		,'inline');
	$id2 = arg(5);

	$userpeer = variable_get("userWimtv");
	$url_live_embedded = variable_get("basePathWimtv") . "liveStream/" . $userpeer . "/" . $userpeer . "/hosts/" . $id2;

   	$credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  	$ch_embedded= curl_init();

    curl_setopt($ch_embedded, CURLOPT_URL, $url_live_embedded);
    curl_setopt($ch_embedded, CURLOPT_VERBOSE, 0);

    curl_setopt($ch_embedded, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch_embedded, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch_embedded, CURLOPT_USERPWD, $credential);
    curl_setopt($ch_embedded, CURLOPT_SSL_VERIFYPEER, FALSE);
    $embedded= curl_exec($ch_embedded);
	$arrayjson_live = json_decode($embedded);
	$url =  $arrayjson_live->url;
	$title = $arrayjson_live->name;
	
	$stream_url = explode ("/",$url);
	$stream_name = $stream_url[count($stream_url)-1];
	$url = "";
	for ($i=1;$i<count($stream_url)-1;$i++){
		$url .= $stream_url[$i] . "/";
	}
	$url = $stream_url[0] . "/" . $url;
	
	$url = substr($url, 0, -1);

$content = '

<div id="page">	<h1>Producer</h1>
  
<p>On this page you can view the video you\'re broadcasting live. Keep it open during the whole transmission.</p>
<div  class="pageproducer">
<div id="producer" ></div>
</div>


<script type="text/javascript">
jQuery(document).ready(function(){ 

	var url_pathPlugin ="' . url(drupal_get_path('module', 'wimtvpro'),array('absolute' => TRUE)) . '";
	var xiSwfUrlStr = url_pathPlugin  + "/jquery/swfObject/playerProductInstall.swf";
	console.log(xiSwfUrlStr );
	var flashvars = {};
    var params = {};
    params.quality = "high";
    params.bgcolor = "#ffffff";
    params.allowscriptaccess = "sameDomain";
    params.allowfullscreen = "true";
    var attributes = {};
    attributes.align = "left";

	swfobject.embedSWF(url_pathPlugin  + "/jquery/swfObject/producer.swf", "producer", "640", "480", "11.4.0",xiSwfUrlStr, flashvars, params, attributes );
	setTimeout(function () {
		producer = jQuery("#producer")[0];
	    console.log(producer);
	    
	    producer.setCredentials("' .  variable_get("userWimtv") . '", "' . variable_get("passWimtv") . '");
	    producer.setUrl(decodeURIComponent("' . $url . '"));
	    producer.setStreamName("' . $stream_name . '");
	    producer.setStreamWidth(640);
	    producer.setStreamHeight(480);
	    producer.connect();
	}, 1000);
    
});
</script>
';

return $content;
}