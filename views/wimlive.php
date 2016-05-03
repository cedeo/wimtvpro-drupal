<?php

/**
 * Created with JetBrains PhpStorm.
 * User: walter
 * Date: 17/12/13
 * Time: 15.03
 */
function wimtvpro_wimlive() {

    $view_page = wimtvpro_alert_reg();
    form_set_error("error", $view_page);
    if ($view_page == "") {
        //View list future event created
        return render_template('templates/wimlive.php', array('elenco' => wimtvpro_elencoLive("all", "table")));
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
    apiDeleteLive($identifier);
    header('Location:' . url('admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink')));
}

//This is a form
function wimtvpro_form($type, $identifier) {
    drupal_add_js('
            //Request new URL for create a wimlive Url
          jQuery(document).ready(function() {
              var timezone = -(new Date().getTimezoneOffset()) * 60 * 1000;
//              var clitimestamp=new Date().getTime();
//              alert(clitimestamp);
              jQuery("#timelivejs").val(timezone);
              jQuery(".createUrl").click(function() {
                  jQuery.ajax({
                      context: this,
                      url: "' . url("admin/config/wimtvpro/wimtvproCallAjax") . '",
                      type: "GET",
                      dataType: "html",
                      data: {
                          namefunction: "urlCreate",
                          titleLive: jQuery("#edit-name").val(),
                      },
                      success: function(response) {                        
                          try {
                              var json = jQuery.parseJSON(response);
                              var result = json.result;
                              if (result === "SUCCESS") {
                                  jQuery("#edit-url").attr("readonly", "readonly");
                                  jQuery("#edit-url").attr("value", json.liveUrl);
                                  jQuery(this).hide();
                                  jQuery(".removeUrl").show();
                              } else {
                                  alert(Drupal.t("Insert a password for live streaming is required"));
                                  jQuery(".passwordUrlLive").show();
                                  jQuery(".createPass").click(function() {
                                      jQuery.ajax({
                                          context: this,
                                          url: "' . url("admin/config/wimtvpro/wimtvproCallAjax") . '",
                                          type: "GET",
                                          dataType: "html",
                                          data: {
                                              namefunction: "passCreate",
                                              newPass: jQuery("#passwordLive").val(),
                                          },
                                          success: function(response) {
                                              alert(response);
                                              jQuery(".passwordUrlLive").hide();
                                          }
                                      });
                                  });
                              }
                          }
                          catch (e) {
//                              alert(e.toString());
                              alert(Drupal.t("Connection error."));
                          }
                      },
                      error: function(request, error) {
                          alert(request);
                      }
                  });
              });
              jQuery(".removeUrl").click(function() {
                  jQuery(this).hide();
                  jQuery(".createUrl").show();
                  jQuery("#edit-url").removeAttr("disabled");
                  jQuery("#edit-url").val("");
              });
          });
	 ', "inline");

    if ($type == "modify") {
        // NS: We use the GETted value "timezone_" and we pass it to apiEmbeddedLive(..)
        // to make it aware about "daylight saving".
        $timezone_ = isset($_GET['timezone_']) ? $_GET['timezone_'] : null;
        $cliTimezoneName = isset($_GET['cliTimezoneName']) ? $_GET['cliTimezoneName'] : "";

//        $dati = apiEmbeddedLive($identifier, $timezone_);
        $dati = apiGetLive($identifier, $timezone_);

        $arraydati = json_decode($dati);

        $name = $arraydati->name;
        if ($arraydati->paymentMode == "FREEOFCHARGE") {
            $payperview = "0";
        } else {
            $payperview = $arraydati->pricePerView;
        }


        $url = $arraydati->url;
        $recordEvent = ($arraydati->recordEvent) ? 'true' : 'false';
        $publicEvent = ($arraydati->publicEvent) ? 'true' : 'false';
        $giorno = $arraydati->eventDate;

        // <#1
        $data = $arraydati->eventDateMillisec;
        $timestamp = floor($data / 1000);
        $start = new DateTime("@$timestamp");

        $cliTimezone = new DateTimeZone($cliTimezoneName);
        $start->setTimezone($cliTimezone);

        $ora = $start->format('H') . ":" . $start->format('i');
        // >#1
///////////////////////
//        $timezone = $arraydati->eventTimeZone;
//        if (intval($arraydati->eventMinute) < 10) {
//            $arraydati->eventMinute = "0" . $arraydati->eventMinute;
//        }
//        $ora = $arraydati->eventHour . ":" . $arraydati->eventMinute;
///////////////////////
        $tempo = $arraydati->duration;


        $ore = floor($tempo / 60);
        $minuti = $tempo % 60;

        $durata = $ore . "h";
        if ($minuti < 10)
            $durata .= "0";
        $durata .= $minuti;
    }
    else {
        // CREATE NEW LIVE EVENT!
        $name = "";
        $payperview = "0";
        $url = "";
        $giorno = "";
        $ora = "";
        $durata = "";
        $publicEvent = "false";
        $recordEvent = "false";
    }
    global $base_url, $base_path, $base_root;
    drupal_add_js("var url_pathPlugin ='" . $base_url . "';", "inline");
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
    drupal_add_js('jQuery(document).ready(function(){jQuery( ".pickaduration" ).timepicker({   defaultTime:"00h00",showPeriodLabels: false,timeSeparator: "h", });});', 'inline');

    $form['htmltag'] = array(
        '#markup' => variable_get('htmltag', l(t("Return event list"), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink')))
    );

    $form['htmltag2'] = array(
        '#markup' => '<p>Here you can create live streaming events to be published on the pages of the site.<br/>
				To use this service you must have installed on your pc a video encoding software (e.g. Adobe Flash Media Live Encoder, Wirecast etc.) or you can broadcast directly from your webcam, simply clicking the icon below under the "Live now" column.<br/>
				By clicking the icon, the producer will open in a new browser tab, keep it open during the whole transmission.</p>');

    $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Title'),
        '#description' => t('Title of the event'),
        '#default_value' => variable_get('name', $name),
        '#size' => 100,
        '#maxlength' => 200,
        '#required' => TRUE,
    );

    $form['payperview'] = array(
        '#type' => 'textfield',
        '#title' => t('Enter the price for viewers to access the event'),
        '#description' => t('Please set a price for viewing your event (set 0 for free of charge). Prices are expressed in €'),
        '#default_value' => variable_get('payperview', $payperview),
        '#size' => 10,
        '#maxlength' => 5,
        '#required' => TRUE,
    );

    $form['Url'] = array(
        '#type' => 'textfield',
        '#title' => t('Url'),
        '#description' => t('URL through which the streaming can be done. <b class="createUrl"> CREATE YOUR URL </b><b id="' . variable_get("userWimtv") . '" class="removeUrl"> REMOVE YOUR URL </b><br/><div class="passwordUrlLive"> Password Live is missing, insert a password for live streaming: <input type="password" id="passwordLive" /> <b class="createPass">Save</b></div>'),
        '#default_value' => variable_get('payperview', $url),
        '#size' => 100,
        '#maxlength' => 800,
        '#required' => TRUE,
    );


    $form['Public'] = array(
        '#type' => 'radios',
        '#title' => t('Event status '),
        '#maxlength' => 5,
        '#options' => array('true' => 'Public', 'false' => 'Private'),
        '#description' => 'If you want to index your event on wimlive.wim.tv, and in WimView (WimTV mobile app) select "Public"',
        '#required' => TRUE,
        '#default_value' => $publicEvent,
    );

    $form['Record'] = array(
        '#type' => 'radios',
        '#title' => t('Record event'),
        '#maxlength' => 5,
        '#options' => array('true' => 'Yes', 'false' => 'No'),
        '#required' => TRUE,
        '#description' => t('Select "Yes" if you want to record your event. The recorded video will appear in WimBox'),
        '#default_value' => $recordEvent,
    );

    $form['Giorno'] = array(
        '#type' => 'textfield',
        '#title' => t('Start date dd/mm/yy'),
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

//    $form['Timezone'] = array(
    $form['eventTimeZone'] = array(
        '#type' => 'select',
        '#title' => t('TimeZone'),
        '#options' => timezoneList(),
        '#required' => TRUE,
        '#default_value' => NULL,
        '#description' => t(''),
    );

    $form['Duration'] = array(
        '#type' => 'textfield',
        '#title' => t('Duration'),
        '#default_value' => $durata,
        '#size' => 10,
        '#maxlength' => 10,
        '#attributes' => array('class' => array('pickaduration')),
        '#required' => TRUE,
    );
    if ($type == "modify") {
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Edit'),
        );
        $form['identifier'] = array(
            '#type' => 'hidden',
            '#default_value' => $identifier,
        );
    } else {
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Add'),
        );
    }
    $form['timelivejs'] = array(
        '#type' => 'hidden',
        '#default_value' => '',
        '#attributes' => array('id' => array('timelivejs')),
    );
    $form['typeValue'] = array(
        '#type' => 'hidden',
        '#default_value' => $type,
    );

    $form['#validate'][] = 'wimtvpro_wimlive_validate';
    $form['#submit'][] = 'wimtvpro_wimlive_submit';
    return $form;
}

function wimtvpro_set_year_range($form_element) {
    $form_element['year']['#options'] = drupal_map_assoc(range(date("Y"), date("Y") + 10));
    return $form_element;
}

function wimtvpro_wimlive_validate($form, &$form_state) {
//    var_dump($_POST);
//       die();
//    $name = check_plain($_POST['name']);
//    $payperview = check_plain($_POST['payperview']);
//    $public = check_plain($_POST['Public']);
//    $record = check_plain($_POST['Record']);
//    if ($payperview == "0")
//        $typemode = "FREEOFCHARGE";
//    else
//        $typemode = "PAYPERVIEW&pricePerView=" . $payperview . "&ccy=EUR";
//
//    $url = check_plain($_POST['Url']);
//    if ($_POST['Giorno'] != "") {
//        $giorno = check_plain($_POST['Giorno']);
//    }
//    else
//        $giorno = "";
//    if ($_POST['Ora'] != "") {
//        $ora = explode(":", check_plain($_POST['Ora']));
//    } else {
//        $ora[0] = "";
//        $ora[1] = "";
//    }
//    if ($_POST['Duration'] != "") {
//        $separe_duration = explode("h", check_plain($_POST['Duration']));
//        $duration = ($separe_duration[0] * 60) + $separe_duration[1];
//    } else {
//        $duration = 0;
//    }
//
//    $params = array("name" => $name,
//        "url" => $url,
//        "eventDate" => $giorno,
//        "paymentMode" => $typemode,
//        "eventHour" => $ora[0],
//        "eventMinute" => $ora[1],
//        "duration" => $duration,
//        "durationUnit" => "Minute",
//        "publicEvent" => $public,
//        "timezone" => $_POST['timelivejs'],
//        "recordEvent" => $record);
//
//    if ($_POST['typeValue'] == "modify")
//        $response = apiModifyLive($_POST['identifier'], $params, $_POST['timelivejs']);
//    else
//        $response = apiAddLive($params, $_POST['timelivejs']);
//
//    if ($response != "") {
//        $message = json_decode($response);
//
//        if (isset($message->result)) {
//            $result = $message->result;
//
//            if (!$result == "SUCCESS") {
//
//                $formset_error = "";
//                foreach ($message->{"messages"} as $key => $value) {
//                    if ($value->message != "")
//                        $formset_error .= $value->field . "=" . $value->message;
//                }
//                form_set_error("", check_plain($formset_error));
//            }
//        }
//        else {
//            form_set_error("", t("Event creation failure. You need to enable \"Live Transmission\" on your wimtv's personal page"));
//        }
//    }
}

function wimtvpro_wimlive_submit($form, &$form_state) {
    $name = check_plain($_POST['name']);
    $payperview = check_plain($_POST['payperview']);
    $public = check_plain($_POST['Public']);
    $record = check_plain($_POST['Record']);

    $ccy = "EUR";
    $pricePerView = "0";

    if ($payperview == "0") {
        $typemode = "FREEOFCHARGE";
        $pricePerView = "0";
    } else {
//        $typemode = "PAYPERVIEW&pricePerView=" . $payperview . "&ccy=EUR";
        $typemode = "PAYPERVIEW";
        $pricePerView = $payperview;
    }

    $url = check_plain($_POST['Url']);
    if ($_POST['Giorno'] != "") {
        $giorno = check_plain($_POST['Giorno']);
    }
    else
        $giorno = "";
    if ($_POST['Ora'] != "") {
        $ora = explode(":", check_plain($_POST['Ora']));
    } else {
        $ora[0] = "";
        $ora[1] = "";
    }
    if ($_POST['Duration'] != "") {
        $separe_duration = explode("h", check_plain($_POST['Duration']));
        $duration = ($separe_duration[0] * 60) + $separe_duration[1];
    } else {
        $duration = 0;
    }

    // GET A PAYMENT CODE FROM SERVER
    $paymentCodeResponse = apiGetUUID();
    $paymentCode = isset($paymentCodeResponse->body) ? $paymentCodeResponse->body : "";

    $params = array(
        "name" => $name,
        "url" => $url,
        "eventDate" => $giorno,
        "paymentMode" => $typemode,
        "eventHour" => $ora[0],
        "eventMinute" => $ora[1],
        "duration" => $duration,
        "durationUnit" => "Minute",
        "publicEvent" => $public,
//        "timezone" => $_POST['timelivejs'],
        "eventTimeZone" => $_POST['eventTimeZone'],
        "recordEvent" => $record,
        "paymentCode" => $paymentCode,
        "pricePerView" => $pricePerView,
        "ccy" => $ccy
    );

    if ($_POST['eventTimeZone'] != "") {
        $timezone = $_POST['eventTimeZone'];
    } else {
        $timezone = $_POST['timelivejs'];
    }

//    if ($_POST['typeValue'] == "modify") {
//        $response = apiModifyLive($_POST['identifier'], $params, $_POST['timelivejs']);
//    } else {
//        $response = apiAddLive($params, $_POST['timelivejs']);
//    }

//    watchdog("wimlive_debug_id", '<pre>' . print_r($_POST['identifier'], true) . '</pre>');
//    watchdog("wimlive_debug_params", '<pre>' . print_r($params, true) . '</pre>');
//    watchdog("wimlive_debug_timezone", '<pre>' . print_r($timezone, true) . '</pre>');
//var_dump($_POST['identifier'], $params, $timezone);die;
    if ($_POST['typeValue'] == "modify") {
        $response = apiModifyLive($_POST['identifier'], $params, $timezone);
    } else {
        $response = apiAddLive($params, $timezone);
    }

    $formset_error = "";
    if ($response != "") {
        $message = json_decode($response);
        if (isset($message->result)) {
            $result = $message->result;
            if ($result !== "SUCCESS") {

                foreach ($message->{"messages"} as $key => $value) {
                    if ($value->message != "")
                        $formset_error .= $value->field . "=" . $value->message;
                }
                form_set_error("", check_plain($formset_error));
            }
        }
        else {
            $formset_error = "Event creation failure. You need to enable \"Live Transmission\" on your wimtv's personal page";
            form_set_error("", t($formset_error));
        }
    }

    if ($formset_error == "") {
        //drupal_set_message(t("Insert event successfully"));
        $form_state['rebuild'] = TRUE;
        drupal_add_js("jQuery(document).ready(function() {
		        window.location ='" . url('admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink')) . "';
				});", "inline");
    }
}

//View event into public page
function wimtvpro_live_public() {
    $output = wimtvpro_elencoLive("0", "video");
    $output .= '<br/><b>UPCOMING EVENTS</b>';
    $output .= "<ul>" . wimtvpro_elencoLive("prev", "list") . "</ul>";
    return $output;
}

//List your future live event
function wimtvpro_elencoLive($number, $type, $onlyActive = true) {
    drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/scripts/jstz-1.0.4.min.js');
    if ($type == "table") {
        $output = 'jQuery("#tableLive tbody").html(response)';
    } else {
        $output = 'jQuery(".live_' . $type . '").html(response)';
    }
    $script =
            'jQuery(document).ready(function(){
             var timezone = -(new Date().getTimezoneOffset())*60*1000;
             var cliTimezoneName = jstz.determine().name();    
             jQuery.ajax({
                 context: this,
                 url:  "' . url("wimtvpro/elencoLive") . '",
                 type: "POST",
                 dataType: "html",
                 async: false,
//                 data: "type=' . $type . '&timezone =" + timezone  + "&id=' . $number . '&onlyActive=' . $onlyActive . '",
                 data: "type=' . $type . '&timezone =" + timezone  + "&id=' . $number . '&onlyActive=' . $onlyActive . '&cliTimezoneName="+ cliTimezoneName,
                 success: function(response) {' . $output . '},
             });
         });';

    drupal_add_js($script, 'inline');
}

function wimtvpro_tableLive() {
    global $base_url, $base_path, $base_root;
//    $timezone = $_POST['timezone_'];
    $timezone = isset($_POST['timezone_']) ? $_POST['timezone_'] : "";
    $cliTimezoneName = isset($_POST['cliTimezoneName']) ? $_POST['cliTimezoneName'] : "";
    $type = $_POST['type'];
    $id = $_POST['id'];
    $onlyActive = $_POST['onlyActive'];
    $userpeer = variable_get("userWimtv");

    $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
    $json = apiGetLiveEvents($timezone, !(!$onlyActive));
    $arrayjson_live = json_decode($json);
    $count = -1;
    $output = "";
    if ($arrayjson_live) {
//        watchdog("wimlive_views_wimlive_array_debug", '<pre>' . print_r($arrayjson_live, true) . '</pre>');
        foreach ($arrayjson_live->hosts as $key => $value) {
            $count++;
            $name = $value->name;
            if (isset($value->url))
                $url = $value->url;
            else
                $url = "";
            if ($value->paymentMode == "FREEOFCHARGE")
                $payperview = "0";
            else
                $payperview = $value->pricePerView;

            $day = $value->eventDate;
            $payment_mode = $value->paymentMode;
            if ($payment_mode == "FREEOFCHARGE")
                $payment_mode = "Free";
            else {
                $payment_mode = $value->pricePerView . " &euro;";
            }
            if ($value->durationUnit == "Minute") {
                $tempo = $value->duration;
                $ore = floor($tempo / 60);
                $minuti = $tempo % 60;
                $durata = $ore . " h ";
                if ($minuti < 10)
                    $durata .= "0";
                $durata .= $minuti . " min";
            }
            else {
                $durata = $value->duration . " " . $value->durationUnit;
            }
            $params = "timezone=" . $_POST['timezone_'];
            $identifier = $value->identifier;
//            $embedded_iframe = apiGetLiveIframe($identifier, $params);

            $embedded_code_content = "[wimlive]" . $identifier . "|" . variable_get("widthPreview") . "|" . variable_get("heightPreview") . "|" . $timezone . "[/wimlive]";
//                    apiGetLiveIframe($identifier, $params);
//            $details_live = apiEmbeddedLive($identifier, $timezone);
            $details_live = apiGetLive($identifier, $timezone);
//            watchdog("wimlive_views_wimlive_details_debug", '<pre>' . print_r($details_live, true) . '</pre>');
            $livedate = json_decode($details_live);

            $data = $livedate->eventDate;
            $millis = $livedate->eventDateMillisec;
            if (intval($livedate->eventMinute) < 10)
                $livedate->eventMinute = "0" . $livedate->eventMinute;
            $oraMin = $livedate->eventHour . ":" . $livedate->eventMinute;
            $timeToStart = $livedate->timeToStart;
            $timeLeft = $livedate->timeLeft;

            $embedded_code = '<textarea readonly="readonly" onclick="this.focus(); this.select();">' . $embedded_code_content . '</textarea>';
            if ($type == "table") {
                $dataNow = date("d/m/Y");
                $dataLive = explode(" ", $day);
                $arrayData = explode("/", $dataLive[0]);
                $arrayOra = explode(":", $dataLive[1]);
                /*
                  $timeStampInizio =  mktime($arrayOra[0],$arrayOra[1],0,$arrayData[1],$arrayData[0],$arrayData[2]);

                  $secondiDurata = 60 * $durata;
                  $ora= date("H:i:s", $secondiDurata);
                  $arrayDurata = explode (":",$ora);

                  $timeStampFine =  mktime($arrayOra[0]+$arrayDurata[0],$arrayOra[1]+$arrayDurata[1],$arrayOra[2]+$arrayDurata[2],$arrayData[1],$arrayData[0],$arrayData[2]);

                  $timeStampNow =  mktime(date("H"),date("i"),date("s"),date("m"),date("d"),date("Y"));
                 */

                $liveIsNow = false;
                if ($timeToStart <= 0 && $timeLeft > 0) {
                    $liveIsNow = true;
                }
                $producer = "";
                if ($liveIsNow)
                    $producer = "<a target='newPage' href='" . url('admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink') . '/webproducer/' . $identifier) . "'  id='" . $identifier . "'><img src='" . $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/img/webcam.png'></a>";

                $output .="<tr>
        <td>" . $name . "</td>
		<td>" . $producer . "</td>
        <td>" . $payment_mode . "</td>
        <td>" . $url . "</td>
        <td>" . $day . " " . $oraMin . "<br/>" . $durata . "</td>
        <td>" . $embedded_code . "</td>" .
                        // NS: We append "timezone_" value as querystring to make "modify" function
                        //  aware about "daylight saving"
//        "<td>" . l(t("Edit"), "admin/config/wimtvpro/wimlive/modify/" . $identifier) . " | " . l(t("Delete"), "admin/config/wimtvpro/wimlive/delete/" . $identifier) . "</td>".
                        "<td>" . l(t("Edit"), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink') . "/modify/" . $identifier, array('query' => array('timezone_' => $timezone, 'cliTimezoneName' => $cliTimezoneName))) . " | " . l(t("Delete"), 'admin/config/' . getWhiteLabel('APP_NAME') . '/' . getWhiteLabel('WIMLIVE_urlLink') . "/delete/" . $identifier) . "</td>" .
                        "</tr>";
            }
            elseif ($type == "list") {
                if (($number == "prev") && ($count == 0))
                    $output .= "";
                elseif (($number == "prev") && ($count > 0))
                    $output .="<li><b>" . $name . "</b> " . $payment_mode . " - " . $data . " " . $oraMin . " - " . $durata . "</li>";
                else
                    $output .="<li><b>" . $name . "</b> " . $payment_mode . " - " . $data . " " . $oraMin . " - " . $durata . "</li>";
            }
            else {
                $name = "<b>" . $name . "</b>";
                $day = "Begins to " . $day;
                $output = $name . "<br/>";
                $output .= $data . " " . $oraMin . "<br/>" . $durata . "<br/>";
                $output .= $embedded_iframe;
            }
            if (($number == "0") && ($count == 0))
                break;
        }
    }
    if ($count < 0) {
        $output = "<tr><td colspan='7'>" . t("No event scheduled at this time") . "</td></tr>";
    }
    echo $output;
}

?>