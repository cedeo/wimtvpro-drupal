<?php
/**
  * @file
  * This file is use for configured form for upload new video.
  *
  */
function wimtvpro_form_alter(&$form, &$form_state, $form_id) {
    /**
     * Permette di inserire contenuti di wim.tv all'interno di un content di Drupal.
     */
  $set_into_video = variable_get('contenttypeWithInsertVideo');
  $field_to_insert = "";
  $exists_insert = FALSE;

  if (isset($form_state['field'])) {
    foreach ($form_state['field'] as $key => $value) {
      if (array_key_exists($key, $set_into_video)) {
	    $key = str_replace("_","-",$key);
        $exists_insert = TRUE;
	    $field_to_insert = "#edit-" . $key . "-und-0-value";
	  }
    }
  }

  if ((strstr($form_id, 'node_form')) && ($exists_insert)) {

      $videos = "<ul class='itemsInsert'>" . wimtvpro_getThumbs(TRUE, FALSE, TRUE) . "</ul><div class='empty'></div>";
      $form['thumbVideo'] = array(
          '#type' => 'item',
          '#title' => t('WimVod'),
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
              var text = "[wimtv]" + jQuery(this).attr("id") + "|" + jQuery(this).parent().children("p").children(".w").val() + "|" + jQuery(this).parent().children("p").children(".h").val() + "[/wimtv]";
              jQuery(this).parent().children("p").children(".w").attr("disabled", "disabled");
              jQuery(this).parent().children("p").children(".h").attr("disabled", "disabled");
              jQuery("' . $field_to_insert . '").insertAtCaret(text);
              jQuery(this).parent().addClass("select");
          });
          jQuery("a.removeThumb").click(function(){
              var testo = jQuery("#edit-body-und-0-value").val() + "";
              testo = testo.replace("[wimtv]" + jQuery(this).attr("id") + "|" + jQuery(this).parent().children("p").children(".w").val() + "|" + jQuery(this).parent().children("p").children(".h").val() + "[/wimtv]","");
              jQuery(this).parent().children("p").children(".w").removeAttr("disabled");
              jQuery(this).parent().children("p").children(".h").removeAttr("disabled");

              jQuery("' . $field_to_insert . '").val(testo);
              jQuery(this).parent().removeClass("select");
          });
      });', 'inline');

    }

    //if (isset($form["#field"]["columns"]["wimvideo"]))
        $form['field']['cardinality']['#options'] = array("1"=>"1");
    //if (isset($form["#field"]["columns"]["wimplaylist"]))
        $form['field']['cardinality']['#options'] = array("1"=>"1");

}

function wimtvpro_node_view_alter(&$build) {
    /**
     * Sostituisce lo 'shortcode' inserito in un content con il rispettivo video embedded, solo quando il contenuto viene visualizzato.
     */
  global $base_url;

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
              if (variable_get('nameSkin')) {
                $directory = file_create_url('public://skinWim');
                $skin = $directory . "/" . variable_get('nameSkin') . ".zip";
              } else {
                $skin = $base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/skin/default.zip";
              }
              $params = "get=1&width=" . $format_video[1] . "&height=" . $format_video[2] . "&skin=" . $skin;
              $response = apiGetPlayerShowtime($contentid, $params); // curl_exec($ch);
              $iframe = $response;
              $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", $iframe, $testoformat);
            }
            else {
              $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", t("The video isn't into WimVod"), $testoformat);
            }
            $build[$valueKey]["#items"][0][$key_testo] = $testoformat;
            $build[$valueKey][0]["#markup"] = $testoformat;
          }
        }
      }
    }
  }
}