<?php
/**
  * @file
  * This file is use for configured form for upload new video.
  *
  */
function wimtvpro_form_alter(&$form, &$form_state, $form_id) {

  $set_into_video = variable_get('contenttypeWithInsertVideo');
  $field_to_insert = "";
  $exists_insert = FALSE;

  if (isset($form_state['field'])) {
    foreach ($form_state['field'] as $key => $value) {
	  
      if (array_key_exists($key, $set_into_video)) {
	    $key = str_replace("_","-",$key);
        $exists_insert = TRUE;
	    $field_to_insert = "#edit-" . $key . "-und-0-value";
	    //exit;
	  }
    }
  }

  if ((strstr($form_id, 'node_form')) && ($exists_insert)) {

  $videos = "<ul class='itemsInsert'>" . wimtvpro_getThumbs(TRUE, FALSE, TRUE) . "</ul><div class='empty'></div>";
  $form['thumbVideo'] = array(
  '#type' => 'item',
  '#title' => t('My Streaming'),
  '#markup' => $videos
  );

  drupal_add_js('
  jQuery(document).ready(function(){

  jQuery.fn.extend({
  insertAtCaret: function(valueToInsertAtCaret){
  return this.each( function(i) {
  if ( document.selection ) {
    this.focus();
    selection = document.selection.createRange();
    selection.text = valueToInsertAtCaret;
    this.focus();
  } else if ( this.selectionStart || this.selectionStart == "0" ) {
    var startPosition = this.selectionStart;
    var endPosition = this.selectionEnd;
    var scrollTop = this.scrollTop;
    this.value = this.value.substring(0, startPosition) + valueToInsertAtCaret + this.value.substring(endPosition, this.value.length);
    this.focus();
    this.selectionStart = startPosition + valueToInsertAtCaret.length;
    this.selectionEnd = startPosition + valueToInsertAtCaret.length;
    this.scrollTop = scrollTop;
  } else {
    this.value += valueToInsertAtCaret;
    this.focus();
  }
  })
  }
  });
  
  var text = jQuery("' . $field_to_insert . '").val();
  
  var n = null;
  if (text!="")
    n= text.match(/\[wimtv](.*?)\[\/wimtv\]/g);

  if (n!=null){
    jQuery.each(n, function(i, val) {
      val=val.replace("[wimtv]","");
      val=val.replace("[/wimtv]","");
      array = val.split("|");
      jQuery("#" + array[0]).parent().parent().parent().addClass("select");
      jQuery("#" + array[0]).parent().parent().children(".w").attr("disabled", "disabled");
       jQuery("#" + array[0]).parent().parent().children(".h").attr("disabled", "disabled");
      jQuery("#" + array[0]).parent().parent().children(".w").val(array[1]);
      jQuery("#" + array[0]).parent().parent().children(".h").val(array[2]);
    });
  }

  jQuery(".wimtv-thumbnail").colorbox({});

  jQuery("a.addThumb").click(function(){
  var text = "[wimtv]" + jQuery(this).attr("id") + "|" + jQuery(this).parent().parent().parent().children(".w").val() + "|" + jQuery(this).parent().parent().parent().children(".h").val() + "[/wimtv]";
  jQuery(this).parent().parent().children(".w").attr("disabled", "disabled");
  jQuery(this).parent().parent().children(".h").attr("disabled", "disabled");
  jQuery("' . $field_to_insert . '").insertAtCaret(text);
  jQuery(this).parent().parent().parent().addClass("select");

  });
  jQuery("a.removeThumb").click(function(){
  var testo = jQuery("#edit-body-und-0-value").val() + "";
  testo = testo.replace("[wimtv]" + jQuery(this).attr("id") + "|" + jQuery(this).parent().parent().parent().children(".w").val() + "|" + jQuery(this).parent().parent().parent().children(".h").val() + "[/wimtv]","");
  jQuery(this).parent().parent().children(".w").removeAttr("disabled");
  jQuery(this).parent().parent().children(".h").removeAttr("disabled");

  jQuery("' . $field_to_insert . '").val(testo);
  jQuery(this).parent().parent().parent().removeClass("select");
  });
  });', 'inline');

}

}

function wimtvpro_node_view_alter(&$build) {
  
  $set_into_video = variable_get('contenttypeWithInsertVideo');
  if (isset($build)) {
    $valueKey = "";
    foreach ($build as $key => $value) {
	  
      if (array_key_exists($key, $set_into_video)) {
	    $valueKey = $key;
        $exists_insert = TRUE;
	    $field_to_insert = "#edit-" . $key . "-und-0-value";
	    //exit;
	  }
    }

  if (isset($build[$valueKey]["#items"][0])) {
    $array_testoformat = $build[$valueKey]["#items"][0];
    foreach ($array_testoformat as $key_testo => $testoformat) {
      preg_match_all("/\[wimtv](.*?)\[\/wimtv\]/msi", $testoformat, $risultato);
      if (isset($risultato[1])) {
        foreach ($risultato[1] as $key => $value) {
          $format_video = explode("|", $value);
          //Recove url video for view it
          $jsonst = wimtvpro_detail_showtime(TRUE, $format_video[0]);
          $arrayjsonst = json_decode($jsonst);
          if (isset($arrayjsonst->{"showtimeIdentifier"})) {
            $showtimeidentifier = $arrayjsonst->{"showtimeIdentifier"};
            $contentid = $arrayjsonst->{"contentId"};
            $ch = curl_init();
            $skin = "public://skinWim/" . variable_get('nameSkin') . ".zip";
            $url = variable_get("basePathWimtv") . variable_get("urlVideosWimtv") . "/" . $contentid . '/embeddedPlayers';
            $url .= "?get=1&width=" . $format_video[1] . "&height=" . $format_video[2] . "&skin=" . $skin;
            $credential = variable_get("userWimtv") . ":" . variable_get("passWimtv");
            curl_setopt($ch, CURLOPT_URL,  $url);
            curl_setopt($ch, CURLOPT_VERBOSE, 0);
            curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
            curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_setopt($ch, CURLOPT_USERPWD, $credential);
            curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
            $response = curl_exec($ch);
            $iframe = $response;
            $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", $iframe, $testoformat);
          }
          else {
            $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", t("The video isn't into My Streaming"), $testoformat);
          }
          $build[$valueKey]["#items"][0][$key_testo] = $testoformat;
          $build[$valueKey][0]["#markup"] = $testoformat;
        }
      }
    }
	}
  }
}