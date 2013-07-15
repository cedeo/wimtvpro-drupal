<?php
/**
  * @file
  * This file is use for congirure a live event.
  *
  */

//LIVE VIDEO
function wimtvpro_wimlive_output() {

$view_page = wimtvpro_alert_reg();
form_set_error("error",$view_page);
if ($view_page==""){
  //View list future event created
  $elenco = wimtvpro_elencoLive("all", "table");
  $output = l(t("Add") . " " . t("new event"), "admin/config/wimtvpro/wimlive/insert");
  

    $output .= "<table id='tableLive'>";
    $output .= "<thead><tr><th>Name</th><th>Live Now</th><th>Pay-Per-View</th><th>URL</th><th>Streaming</th><th>Embed Code</th><th></th></tr></thead>";
    $output .= "<tbody>";
    $output .= $elenco;
    $output .= "</tbody>";
    $output .= "</table>";

  
  return $output;
} 
return $view_page;
}

//Form for add new event live
function wimtvpro_wimlive_form($form_state) {
  return wimtvpro_form("insert", "");
}

//Form for modify event live
function wimtvpro_wimlive_formModify($form_state, $id) {
  return wimtvpro_form("modify", $id['build_info']['args'][0]);
}

//Call a delete event live
function wimtvpro_wimlive_delete($form_state, $id) {
  $identifier = $id['build_info']['args'][0];
  $userpeer = variable_get("userWimtv");
  $url_live = variable_get("basePathWimtv") . "liveStream/" . $userpeer . "/" . $userpeer . "/hosts";
  $url_live .= "/" . $identifier;
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url_live);
  curl_setopt($ch, CURLOPT_VERBOSE, 0);
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($ch, CURLOPT_USERPWD, $credential);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
  $response = curl_exec($ch);
  curl_close($ch);
  header('Location:' . url("admin/config/wimtvpro/wimlive"));
}

//This is a form
function wimtvpro_form($type, $identifier) {

  drupal_add_js('
  //Request new URL for create a wimlive Url
  jQuery(document).ready(function(){
	var timezone = -(new Date().getTimezoneOffset())*60*1000;
	jQuery("#timelivejs").val(timezone);              
	jQuery(".createUrl").click(function(){
	  jQuery.ajax({
			context: this,
			url:  "' . url("admin/config/wimtvpro/wimtvproCallAjax") . '",
			type: "GET",
			dataType: "html",
			data:{
				namefunction: "urlCreate",
				titleLive: jQuery("#edit-name").val(),
			},
			success: function(response) {
			  var json =  jQuery.parseJSON(response);
			  var result = json.result;
			  if (result=="SUCCESS"){
			  	jQuery("#edit-url").attr("readonly", "readonly");
			  	jQuery("#edit-url").attr("value", json.liveUrl);
			  	jQuery(this).hide();
				jQuery(".removeUrl").show();
			  } else {
			    //alert (response);
			    alert(Drupal.t("Insert a password for live streaming is required"));
			    jQuery(".passwordUrlLive").show();
			    jQuery(".createPass").click(function(){
			     jQuery.ajax({
			     context: this,
			     url:  "' . url("admin/config/wimtvpro/wimtvproCallAjax") . '",
			     type: "GET",
			     dataType: "html",
			     data:{
				  namefunction: "passCreate",
				  newPass: jQuery("#passwordLive").val(),
			     },
                 success: function(response) {
                 	alert (response);
                 	jQuery(".passwordUrlLive").hide();
                 }
			    });
	            });
			  }  
			},
			error: function(request,error) {
				alert(request);
			}
		});
     });
   jQuery(".removeUrl").click(function(){
     jQuery(this).hide();
     jQuery(".createUrl").show();
     jQuery("#edit-url").removeAttr("disabled");
     jQuery("#edit-url").val("");
   });
 });
	
	 ', "inline");

  if ($type=="modify") {
    $userpeer = variable_get("userWimtv");
    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
    $url_live = variable_get("basePathWimtv") . "liveStream/" . $userpeer . "/" . $userpeer . "/hosts/" . $identifier . "/embed";
    $ch_embedded = curl_init();
    curl_setopt($ch_embedded, CURLOPT_URL, $url_live);
    curl_setopt($ch_embedded, CURLOPT_VERBOSE, 0);
    curl_setopt($ch_embedded, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch_embedded, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch_embedded, CURLOPT_USERPWD, $credential);
    curl_setopt($ch_embedded, CURLOPT_SSL_VERIFYPEER, FALSE);
    $dati = curl_exec($ch_embedded);
    $arraydati = json_decode($dati);
    $name = $arraydati->name;
    if ($arraydati->paymentMode=="FREEOFCHARGE")
      $payperview = "0";
    else
      $payperview =  $arraydati->pricePerView;
	  

    $url = $arraydati->url;

   $giorno = $arraydati->eventDate;

   //$timezone = $arraydati->eventTimeZone;
   if (intval($arraydati->eventMinute)<10) $arraydati->eventMinute = "0" .  $arraydati->eventMinute;
   $ora = $arraydati->eventHour . ":" . $arraydati->eventMinute;
   $tempo = $arraydati->duration;
   $public = $arraydati->publicEvent;
   $recordEvent = $arraydati->recordEvent;
   $ore = floor($tempo / 60);
   $minuti = $tempo % 60;
   
   $durata = $ore . "h";
   if ($minuti<10)
		$durata .= "0";
   $durata .= $minuti;
	
	if ($public) {
		$public_res = "true";
	}
	else {
		$public_res = "false";
	}
	
	if ($recordEvent) {
		$recordEvent_res = "true";
	}
	else {
		$recordEvent_res = "false";
	}
	
  }
  else {
    $name = "";
    $payperview = "0";
    $url = "";
    $giorno = "";
    $ora = "";
    $durata = "";
	$public = "";
	$recordEvent = "";
	$public_res = "";
	$recordEvent_res = "";
  }
  global $base_url,$base_path,$base_root;
drupal_add_js("var url_pathPlugin ='" . $base_url . "';" , "inline");
  drupal_add_library('system', 'ui.datepicker');
  drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickadate" ).datepicker({
      dateFormat: "dd/mm/yy",
      autoSize: true,
      minDate: 0,
    });});', 'inline');
  drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/jquery/timepicker/jquery.ui.timepicker.js');
  drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/wimtvpro.js');
  drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/jquery/timepicker/jquery.ui.timepicker.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  drupal_add_css(drupal_get_path('module', 'wimtvpro') . '/css/wimtvpro.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));

  drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickatime" ).timepicker({  defaultTime:"00:00"  });});', 'inline');
  drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickaduration" ).timepicker({   defaultTime:"00h05",showPeriodLabels: false,timeSeparator: "h", });});', 'inline');

  $form['htmltag'] = array(
  '#markup' => variable_get('htmltag', l(t("Return event list"), "admin/config/wimtvpro/wimlive"))
  );

  $form['htmltag2'] = array(
  '#markup' => '<p>Here you can create live streaming events to be published on the pages of the site.<br/>
				To use this service you must have installed on your pc a video encoding software (e.g. Adobe Flash Media Live Encoder, Wirecast etc.) or you can broadcast directly from your webcam, simply clicking the icon below under the "Live now" column.<br/>
				By clicking the icon, the producer will open in a new browser tab, keep it open during the whole transmission.</p>');

$form['name'] = array(
'#type' => 'textfield',
'#title' => t('Name'),
'#description' => t('Name of the event'),
'#default_value' => variable_get('name', $name),
'#size' => 100,
'#maxlength' => 200,
'#required' => TRUE,
);

$form['payperview'] = array(
'#type' => 'textfield',
'#title' => t('Set the event access'),
'#description' => t('0 as free of charge or you can decide the price of access for each viewer (in &euro;).'),
'#default_value' => variable_get('payperview', $payperview),
'#size' => 10,
'#maxlength' => 5,
'#required' => TRUE,
);

$form['Url'] = array(
'#type' => 'textfield',
'#title' => t('Url'),
'#description' => t('URL through which the streaming can be done. <b class="createUrl"> CREATE YOUR URL </b><b id="' . variable_get("userWimtv") . '" class="removeUrl"> REMOVE YOUR URL </b><br/><div class="passwordUrlLive"> Password Live is missing, insert a password for live streaming: <input type="password" id="passwordLive" /> <b class="createPass">Save</b></div>'),
'#default_value' =>  variable_get('payperview', $url),
'#size' => 100,
'#maxlength' => 800,
'#required' => TRUE,
);


$form['Public'] = array(
    '#type' => 'radios',
    '#title' => t('Event Public or Private'),
    '#maxlength' => 5,
    '#options' => array( 'true' => 'Public', 'false' => 'Private'),
	'#description' => 'If you want to index your event on the website <a target="_blank" href="http://wimlive.wim.tv">wimlive.wim.tv</a>, select the "Public" option.</div>',
    '#required' => TRUE,
    '#default_value' => $public_res,
);

$form['Record'] = array(
    '#type' => 'radios',
    '#title' => t('Wound you like to Record this event?'),
    '#maxlength' => 5,
    '#options' => array( 'true' => 'Yes', 'false' => 'No'),
    '#required' => TRUE,
    '#default_value' => $recordEvent_res,
);

$form['Giorno'] = array(
    '#type' => 'textfield',
    '#title' => t('Date of the event dd/mm/yy'),
    '#size' => 10,
    '#maxlength' => 10,
    '#attributes' => array('class' => array('pickadate')),
    '#required' => TRUE,
    '#default_value' => $giorno,
);

$form['Ora'] = array(
    '#type' => 'textfield',
    '#title' => t('Start time'),
    '#description' => t('We recommend applying a tolerance on the start time to facilitate payment transactions to the viewers.'),
    '#size' => 10,
    '#maxlength' => 10,
    '#attributes' => array('class' => array('pickatime')),
    '#required' => TRUE,
    '#default_value' => $ora,

);
/*
$form['Timezone'] = array(
    '#type' => 'select',
    '#title' => t('TimeZone'),
    '#description' => t('We recommend applying a tolerance on the start time to facilitate payment transactions to the viewers.'),
);*/
$form['Duration'] = array(
'#type' => 'textfield',
'#title' => t('Event duration'),
'#default_value' => $durata,
    '#size' => 10,
    '#maxlength' => 10,
    '#attributes' => array('class' => array('pickaduration')),
     '#required' => TRUE,
);
if ($type=="modify") {
$form['submit'] = array(
'#type' => 'submit',
'#value' => t('Edit'),
);
$form['identifier'] = array(
    '#type' => 'hidden',
    '#default_value' => $identifier,
);
$form['timelivejs'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
	'#attributes' => array('id' => array('timelivejs')),
);
}
else {
$form['submit'] = array(
'#type' => 'submit',
'#value' => t('Add'),
);
}

$form['typeValue'] = array(
    '#type' => 'hidden',
    '#default_value' => $type,
);

$form['#validate'][] = 'wimtvpro_wimlive_validate';
$form['#submit'][] = 'wimtvpro_wimlive_submit';
return $form;

}

function wimtvpro_set_year_range($form_element) {
  $form_element['year']['#options'] = drupal_map_assoc(range(date("Y"), date("Y")+10));
  return $form_element;
}


function wimtvpro_wimlive_validate($form, &$form_state) {
  $name = check_plain($_POST['name']);
  $payperview = check_plain($_POST['payperview']);
  $public = check_plain($_POST['Public']);
  $record = check_plain($_POST['Record']);
  if ($payperview=="0")
    $typemode = "FREEOFCHARGE";
  else
    $typemode = "PAYPERVIEW&pricePerView=" . $payperview . "&ccy=EUR";
  
  $url = check_plain($_POST['Url']);
  if ($_POST['Giorno']!="") {
    $giorno = check_plain($_POST['Giorno']);
  }
  else
    $giorno = "";
  if ($_POST['Ora']!="") {
    $ora = explode(":", check_plain($_POST['Ora']));
  }
  else {
    $ora[0] = "";
    $ora[1] = "";
  }
  if ($_POST['Duration']!="") {
    $separe_duration = explode("h", check_plain($_POST['Duration']));
    $duration = ($separe_duration[0] * 60) + $separe_duration[1];
  }
  else {
    $duration = 0;
  }
  $userpeer = variable_get("userWimtv");
  //url-ify the data for the POST
  $fields_string = "name=" . $name . "&url=" . $url . "&eventDate=" . $giorno . "&paymentMode=" . $typemode;
  $fields_string .= "&eventHour=" . $ora[0] . "&eventMinute=" . $ora[1] . "&duration=" . $duration . "&durationUnit=Minute&publicEvent=" . $public . "&recordEvent=" . $record;
  
  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $url_live = variable_get("basePathWimtv") . "liveStream/" . $userpeer . "/" . $userpeer . "/hosts";
  if ($_POST['typeValue']=="modify")  $url_live .= "/" . $_POST['identifier'];
  $url_live .= "?timezone=" . $_POST['timelivejs'];

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url_live);
  curl_setopt($ch, CURLOPT_USERPWD, $credential);
  curl_setopt($ch, CURLOPT_POST, TRUE);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
  $response = curl_exec($ch);
  curl_close($ch);
  if ($response!="") {
    $message = json_decode($response);
	
	if (isset($message->result)) {
      $result = $message->result;
	
      if (!$result=="SUCCESS") {
        
        $formset_error = "";
        foreach ($message->{"messages"} as $key => $value) {
          if ($value->message!="")
            $formset_error .= $value->field . "=" . $value->message;
        }
        form_set_error("", check_plain($formset_error));
      }
	}
	else {
	  form_set_error("", t("Event creatrion failure. You need to enable \"Live Transmission\" on your wimtv's personal page"));
	}
  }
}

function wimtvpro_wimlive_submit($form, &$form_state) {
  //drupal_set_message(t("Insert event successfully"));
  $form_state['rebuild'] = TRUE;
  drupal_add_js("jQuery(document).ready(function() { 
		        window.location ='" . url("admin/config/wimtvpro/wimlive") . "'; 
				});","inline");

}

//View event into public page
function wimtvpro_live_public() {
  $output = wimtvpro_elencoLive("0", "video");
  $output .= '<br/><b>UPCOMING EVENTS</b>';
  $output .= "<ul>" . wimtvpro_elencoLive("prev", "list") . "</ul>";
  return $output;
}

//List your future live event
function wimtvpro_elencoLive($number, $type, $onlyActive=true) {
  $output = 'jQuery(document).ready(function(){';

    $output .= '
	  var timezone = -(new Date().getTimezoneOffset())*60*1000;
      jQuery.ajax({
	  context: this,
	  url:  "' . url("wimtvpro/elencoLive") . '", 
	  type: "POST",
      dataType: "html",
      async: false,
      data: "type='. $type . '&timezone =" + timezone  + "&id=' . $number . '&onlyActive=' . $onlyActive . '",  
      success: function(response) {';
      if ($type=="table") {
       $output .= 'jQuery("#tableLive tbody").html(response)';
      } else {
        $output .= 'jQuery(".live_' . $type . '").html(response)';
      }
      $output .= '},});});';
      drupal_add_js($output,'inline');
}

function wimtvpro_tableLive() {
  global $base_url,$base_path,$base_root;
  $timezone = $_POST['timezone'];
  $type = $_POST['type'];
  $id =  $_POST['id'];
  $onlyActive = $_POST['onlyActive']; 
  $userpeer = variable_get("userWimtv");
  $url_live_select = variable_get("basePathWimtv") . "liveStream/" . $userpeer . "/" . $userpeer . "/hosts";
  if ($onlyActive)  $url_live_select .= "?active=true";

  $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
  $ch_select = curl_init();

  curl_setopt($ch_select, CURLOPT_URL, $url_live_select);
  curl_setopt($ch_select, CURLOPT_VERBOSE, 0);
  curl_setopt($ch_select, CURLOPT_RETURNTRANSFER, TRUE);
  curl_setopt($ch_select, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
  curl_setopt($ch_select, CURLOPT_USERPWD, $credential);
  curl_setopt($ch_select, CURLOPT_SSL_VERIFYPEER, FALSE);
  $json  =curl_exec($ch_select);
  $arrayjson_live = json_decode($json);
  $count = -1;
  $output = "";
  if ($arrayjson_live ) {
    foreach ($arrayjson_live->hosts as $key => $value) {
      $count ++;
      $name = $value -> name;
      if (isset($value -> url))
        $url =  $value -> url;
      else
        $url = "";
 if ($value->paymentMode=="FREEOFCHARGE") 
   $payperview = "0";
  else
   $payperview =  $value->pricePerView;
  
    $day =  $value -> eventDate;
    $payment_mode =  $value -> paymentMode;
    if ($payment_mode=="FREEOFCHARGE") $payment_mode="Free";
    else {
      $payment_mode=  $value->pricePerView . " &euro;";
    }
    if ( $value -> durationUnit=="Minute") {
   		$tempo = $value->duration;
		$ore = floor($tempo / 60);
		$minuti = $tempo % 60;
		$durata = $ore . " h ";
		if ($minuti<10)
		  $durata .= "0";
		$durata .= $minuti . " min";
	}
	else
		 $durata =  $value->duration . " " . $value -> durationUnit;

  $identifier = $value -> identifier;
  $url_live_embedded = variable_get("basePathWimtv") . "liveStream/" . $userpeer . "/" . $userpeer . "/hosts/" . $identifier . "/embed?timezone=" . $timezone;

  $header[] = "Accept: text/xml,application/xml,application/xhtml+xml,";
  if (variable_get('nameSkin')!="") {
    $directory = file_create_url('public://skinWim');
	    $skin = "&skin=" . $directory . "/" . variable_get('nameSkin') . ".zip";
      }
      else
	    $skin = $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/skin/default.zip";
	  
	  $url_live_embedded .= "?skin=" . $skin;
      $ch_embedded = curl_init();
      curl_setopt($ch_embedded, CURLOPT_URL, $url_live_embedded);
      curl_setopt($ch_embedded, CURLOPT_VERBOSE, 0);
      curl_setopt($ch_embedded, CURLOPT_HTTPHEADER, $header);
      curl_setopt($ch_embedded, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch_embedded, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
      curl_setopt($ch_embedded, CURLOPT_USERPWD, $credential);
      curl_setopt($ch_embedded, CURLOPT_SSL_VERIFYPEER, FALSE);
      $embedded_iframe = curl_exec($ch_embedded);
	$ch_details = curl_init();  
	curl_setopt($ch_details, CURLOPT_URL, $url_live_embedded);
    curl_setopt($ch_details, CURLOPT_VERBOSE, 0);
    curl_setopt($ch_details, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch_details, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_setopt($ch_details, CURLOPT_USERPWD, $credential);
    curl_setopt($ch_details, CURLOPT_SSL_VERIFYPEER, FALSE);
    $details_live = curl_exec($ch_details);
    $livedate = json_decode($details_live);
    
	$data = $livedate->eventDate;
	if (intval($livedate->eventMinute)<10) $livedate->eventMinute = "0" .  $livedate->eventMinute;
	$oraMin = $livedate->eventHour . ":" . $livedate->eventMinute;
	$timeToStart= $livedate->timeToStart;
	$timeLeft = $livedate->timeLeft;


	  
      $embedded_code = '<textarea readonly="readonly" onclick="this.focus(); this.select();">' . $embedded_iframe . '</textarea>';
      if ($type=="table") {
	  
	   
	   $dataNow = date("d/m/Y"); 
      $dataLive = explode(" ",$day);
      $arrayData = explode ("/",$dataLive[0]);
	  $arrayOra = explode (":",$dataLive[1]);
     /*
      $timeStampInizio =  mktime($arrayOra[0],$arrayOra[1],0,$arrayData[1],$arrayData[0],$arrayData[2]);
      
      $secondiDurata = 60 * $durata;
      $ora= date("H:i:s", $secondiDurata);
      $arrayDurata = explode (":",$ora);
    
      $timeStampFine =  mktime($arrayOra[0]+$arrayDurata[0],$arrayOra[1]+$arrayDurata[1],$arrayOra[2]+$arrayDurata[2],$arrayData[1],$arrayData[0],$arrayData[2]);

      $timeStampNow =  mktime(date("H"),date("i"),date("s"),date("m"),date("d"),date("Y"));
*/
 	  
      $liveIsNow = false;
      if ($dataNow == $dataLive[0]){
      //if (($timeStampNow>=$timeStampInizio) && ($timeStampNow<$timeStampFine )) {
			 $liveIsNow = true;
      }
	  $producer = "";
	  if ($liveIsNow)
	  
	    $producer ="<a target='newPage' href='" . url("admin/config/wimtvpro/wimlive/webproducer/" . $identifier) . "'  id='" . $identifier . "'><img src='" .  $base_url  . "/" . drupal_get_path('module', 'wimtvpro') . "/img/webcam.png'></a>";
		
        $output .="<tr>
        <td>" . $name . "</td>
		<td>" . $producer . "</td>
        <td>" . $payment_mode . "</td>
        <td>" . $url . "</td>
        <td>"  . $day . " " . $oraMin . "<br/>" . $durata . "</td>
        <td>" . $embedded_code . "</td>
        <td>" . l(t("Edit"), "admin/config/wimtvpro/wimlive/modify/" . $identifier ) . " | " . l(t("Delete"), "admin/config/wimtvpro/wimlive/delete/" . $identifier ) . "</td>
        </tr>";
      }
      elseif ($type=="list") {
        if (($number=="prev") && ($count==0)) $output .= "";
        elseif (($number=="prev") && ($count>0)) $output .="<li><b>" . $name . "</b> " . $payment_mode . " - " . $data . " " . $oraMin . " - " . $durata . "</li>";
        else $output .="<li><b>" . $name . "</b> " . $payment_mode . " - " . $data . " " . $oraMin   . " - " . $durata . "</li>";


      }
      else {
        $name = "<b>" . $name . "</b>";
        $day =  "Begins to " . $day;
        $output = $name . "<br/>";
         $output .= $data . " " . $oraMin  . "<br/>" . $durata . "<br/>";
        $output .= $embedded_iframe;
      }
      if (($number=="0") && ($count==0)) break;
    }
  }
  if ($count<0) {
    $output = t("No event scheduled at this time");
  }
  echo $output;
}