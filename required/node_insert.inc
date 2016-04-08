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
                $key = str_replace("_", "-", $key);
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

        drupal_add_js("var field_to_insert='" . $field_to_insert . "'", 'inline');
        drupal_add_js(drupal_get_path('module', 'wimtvpro') . '/scripts/wimtvpro_node_insert.js');
    }

    if (isset($form["#field"]["columns"]["wimvideo"])) {
        $form['field']['cardinality']['#options'] = array("1" => "1");
    }
    if (isset($form["#field"]["columns"]["wimplaylist"])) {
        $form['field']['cardinality']['#options'] = array("1" => "1");
    }
}

function wimtvpro_node_view_alter(&$build) {
    global $base_url;
    global $user;
    if (is_array($user->roles) && in_array('administrator', $user->roles)) {
        $cssFile = '/css/wimtvpro.css';
    } else {
        $cssFile = '/css/wimtvpro_public.css';
    }
    drupal_add_css(drupal_get_path('module', 'wimtvpro') . $cssFile, array('group' => CSS_DEFAULT, 'every_page' => TRUE));

    $set_into_video = variable_get('contenttypeWithInsertVideo');
    if (isset($build)) {
        $valueKey = "";
        foreach ($build as $key => $value) {
            if (array_key_exists($key, $set_into_video)) {
                $valueKey = $key;
                $exists_insert = TRUE;
                $field_to_insert = "#edit-" . $key . "-und-0-value";
            }
        }

        if (isset($build[$valueKey]["#items"][0])) {
            $array_testoformat = $build[$valueKey]["#items"][0];
            foreach ($array_testoformat as $key_testo => $testoformat) {
//            $key_testo = 'value';
//            $testoformat = $array_testoformat['value'];
//            
                // SEARCH FOR WIMTV VIDEOS
                $preg_wimtv = preg_match_all("/\[wimtv](.*?)\[\/wimtv\]/msi", $testoformat, $risultato_wimtv);
                if (isset($risultato_wimtv[1])) {
                    $testoformat = wimtvpro_shortcode_wimtv($valueKey, $risultato_wimtv, $key_testo, $testoformat);
                }

                // SEARCH FOR WIMTV PROGRAMMING
//                $preg_wimprog = preg_match_all("/\[wimprog id=\"(.*?)\"\]/msi", $testoformat, $risultato_prog);
//                if (isset($risultato_prog[1])) {
//                    $testoformat = wimtvpro_shortcode_wimprog($valueKey, $risultato_prog, $key_testo, $testoformat);
//                }
                $preg_wimprog = preg_match_all("/\[wimprog](.*?)\[\/wimprog\]/msi", $testoformat, $risultato_prog);
                if (isset($risultato_prog[1])) {
                    $testoformat = wimtvpro_shortcode_wimprog($valueKey, $risultato_prog, $key_testo, $testoformat);
                }

                // SEARCH FOR WIMTV PLAYLIST
                $preg_playlistWimtv = preg_match_all("/\[playlistWimtv](.*?)\[\/playlistWimtv\]/msi", $testoformat, $risultato_playlistWimtv);
                if (isset($risultato_playlistWimtv[1])) {
                    $testoformat = wimtvpro_shortcode_playlistWimtv($valueKey, $risultato_playlistWimtv, $key_testo, $testoformat);
                }

                // SEARCH FOR WIMTV LIVE
                $preg_wimlive = preg_match_all("/\[wimlive](.*?)\[\/wimlive\]/msi", $testoformat, $risultato_wimlive);
                if (isset($risultato_wimlive[1])) {
                    $testoformat = wimtvpro_shortcode_wimlive($valueKey, $risultato_wimlive, $key_testo, $testoformat);
                }

                if ($preg_wimtv || $preg_wimprog || $preg_playlistWimtv || $preg_wimlive) {
                    $build[$valueKey]["#items"][0][$key_testo] = $testoformat;
                    $build[$valueKey][0]["#markup"] = $testoformat;
                }
            }
        }
    }
}

function wimtvpro_shortcode_wimtv($valueKey, $risultato, $key_testo, $testoformat) {
    if (isset($risultato[1])) {
        foreach ($risultato[1] as $key => $value) {
            $format_video = explode("|", $value);
            //Recove url video for view it
            $jsonst = wimtvpro_detail_showtime(TRUE, $format_video[0]);
            $arrayjsonst = json_decode($jsonst);
            if (isset($arrayjsonst->{"showtimeIdentifier"})) {
                $showtimeidentifier = $arrayjsonst->{"showtimeIdentifier"};
                $contentid = $arrayjsonst->{"contentId"};
                $insecureMode = "&insecureMode=on";
                $skin = "";
                $logo = "";
                // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
                $skinData = wimtvpro_get_skin_data();
                if ($skinData['styleUrl'] != "") {
                    $skin = "&skin=" . htmlentities($skinData['styleUrl']);
                }

                if ($skinData['logoUrl'] != "") {
                    $logo = "&logo=" . htmlentities($skinData['logoUrl']);
                }

                $width = (isset($format_video[1]) && $format_video[1] != "") ? $format_video[1] : variable_get("widthPreview");
                $height = (isset($format_video[2]) && $format_video[2] != "") ? $format_video[2] : variable_get("heightPreview");


                $params = "get=1&width=" . $width . "&height=" . $height . $insecureMode . $skin . $logo;

                $response = apiGetPlayerShowtime($contentid, $params); // curl_exec($ch);
                // NS: the restored parameters dont give the expected resize behaviour
                // hence we mangle the response iframe on the fly
                $pattern = "/width=\"(\d+)\" height=\"(\d+)\"/";
                $replacement = "width=\"" . $format_video[1] . "\" height=\"" . $format_video[2] . "\"";
                $response = preg_replace($pattern, $replacement, $response);
                $iframe = $response;
                $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", $iframe, $testoformat);
            } else {
                $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", t("The video isn't into WimVod"), $testoformat);
            }
        }
    }
    return $testoformat;
}

function wimtvpro_shortcode_wimprog($valueKey, $risultato, $key_testo, $testoformat) {
    if (isset($risultato[1])) {
        foreach ($risultato[1] as $key => $value) {
            $format_video = explode("|", $value);
            $progID = isset($format_video[0]) ? $format_video[0] : "";
            $width = isset($format_video[1]) ? $format_video[1] : variable_get("widthPreview");
            $height = isset($format_video[2]) ? $format_video[2] : variable_get("heightPreview");
            if (isset($progID)) {
                $skinData = wimtvpro_get_skin_data();
                $skinStyle = "";
                $skinLogo = "";
                if ($skinData['styleUrl'] != "") {
                    $skinStyle = $skinData["styleUrl"];
                }

                if ($skinData['logoUrl'] != "") {
                    $skinLogo = $skinData['logoUrl'];
                }

                $height = ($height != null) ? $height : get_option("wp_heightPreview") + 100;
                $width = ($width != null) ? $width : get_option("wp_widthPreview");

                $parameters = "";
                $parameters.="width=" . $width;
                $parameters.="&height=" . $height;
                $parameters.="&insecureMode=on";
                $parameters.="&skin=" . $skinStyle;
                $parameters.="&logo=" . $skinLogo;
                $iframe = apiProgrammingPlayer($progID, $parameters);

                $testoformat = str_replace("[wimprog]" . $value . "[/wimprog]", $iframe, $testoformat);
            } else {
                $testoformat = str_replace("[wimprog]" . $value . "[/wimprog]", t("Program not found."), $testoformat);
            }
        }
    }
    return $testoformat;

//    // ORIGINAL
//    if (isset($risultato[1])) {
//        foreach ($risultato[1] as $key => $progId) {
//            $iframe = wimtvpro_programming_embedded($progId);
//            $testoformat = str_replace("[wimprog id=\"" . $progId . "\"]", $iframe, $testoformat);
//        }
//    }
//    return $testoformat;
}

function wimtvpro_shortcode_playlistWimtv($valueKey, $risultato, $key_testo, $testoformat) {
    if (isset($risultato[1])) {
        foreach ($risultato[1] as $key => $value) {
            $format_video = explode("|", $value);
            $playlistID = isset($format_video[0]) ? $format_video[0] : "";
            $width = isset($format_video[1]) ? $format_video[1] : variable_get("widthPreview");
            $height = isset($format_video[2]) ? $format_video[2] : variable_get("heightPreview");

            if (isset($playlistID)) {
                $JwPlayerScript = $GLOBALS['base_url'] . "/" . drupal_get_path('module', 'wimtvpro') . "/jquery/jwplayer/jwplayer.js";
                drupal_add_js($JwPlayerScript, 'external');
                $playerScript = configurePlayer_PlaylistJS($playlistID, $width, $height);
                $testoformat = str_replace("[playlistWimtv]" . $value . "[/playlistWimtv]", $playerScript, $testoformat);
            } else {
                $testoformat = str_replace("[playlistWimtv]" . $value . "[/playlistWimtv]", t("Playlist not found."), $testoformat);
            }
        }
    }
    return $testoformat;
}

function wimtvpro_shortcode_wimlive($valueKey, $risultato, $key_testo, $testoformat) {
    if (isset($risultato[1])) {
        foreach ($risultato[1] as $key => $value) {
            $format_video = explode("|", $value);
            $liveID = isset($format_video[0]) ? $format_video[0] : "";
            $width = isset($format_video[1]) ? $format_video[1] : variable_get("widthPreview");
            $height = isset($format_video[2]) ? $format_video[2] : variable_get("heightPreview");
            $timezone = isset($format_video[3]) ? $format_video[3] : "";

            if (isset($liveID)) {
                $params="";
                $insecureMode = "&insecureMode=on";
                $skin = "";
                $logo = "";
                // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
                $skinData = wimtvpro_get_skin_data();
                if ($skinData['styleUrl'] != "") {
                    $skin = "&skin=" . htmlentities($skinData['styleUrl']);
                }

                if ($skinData['logoUrl'] != "") {
                    $logo = "&logo=" . htmlentities($skinData['logoUrl']);
                }

                $params .="&width=$width&height=$height&timezone=" . $timezone . $insecureMode . $skin . $logo;

                $iframe = apiGetLiveIframe($liveID, $params);
                $testoformat = str_replace("[wimlive]" . $value . "[/wimlive]", $iframe, $testoformat);
            } else {
                $testoformat = str_replace("[wimlive]" . $value . "[/wimlive]", t("Live not found."), $testoformat);
            }
        }
    }
    return $testoformat;


    ///// WP
    $pageLive = "";
    // [wimlive id='urn:wim:tv:livestream:c9309ad5-6cce-4f20-b9aa-552efe858fe4' zone='3600000']
    // WE ARE GETTING A SHORTAG LIKE: [wimlive id='urn:wim:tv:livestream:c9309ad5-6cce-4f20-b9aa-552efe858fe4' zone='3600000']
    if (isset($atts['id']) && isset($atts['zone'])) {
        $identifier = $atts["id"];
        $timezone = $atts["zone"];
        $width = $atts["width"];
        $height = $atts["height"];

        $insecureMode = "&insecureMode=on";
        $skin = "";
        $logo = "";
        // A SKIN HAS BEEN ADDED: OVERRIDE DEFAULT SKIN PATH
        $skinData = wimtvpro_get_skin_data();
        if ($skinData['styleUrl'] != "") {
            $skin = "&skin=" . htmlentities($skinData['styleUrl']);
        }

        if ($skinData['logoUrl'] != "") {
            $logo = "&logo=" . htmlentities($skinData['logoUrl']);
        }

        $params .="&width=$width&height=$height&timezone=" . $timezone . $insecureMode . $skin . $logo;

        $embedded_iframe = apiGetLiveIframe($identifier, $params);
        $pageLive = $embedded_iframe;
    }
    return $pageLive;
}

/*
function wimtvpro_node_view_alter(&$build) {
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
                                // NS: IN THIS CASE WE PASS NO SKIN (see also wimtvpro_embedded()
                                // in: wimtvpro/required/embedded.inc
                                //$base_url . "/" . drupal_get_path('module', 'wimtvpro') . "/skin/default.zip";
                                $skin = "";
                            }
                            // NS: WE HAVE RESTORED THE PREVIOUS (commented) PARAMETERS
                            // $params = "get=1&skin=".$skin;
                            $params = "get=1&width=" . $format_video[1] . "&height=" . $format_video[2] . "&skin=" . $skin;

                            $response = apiGetPlayerShowtime($contentid, $params); // curl_exec($ch);
                            $iframe = $response;
                            $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", $iframe, $testoformat);
                        } else {
                            $testoformat = str_replace("[wimtv]" . $value . "[/wimtv]", t("The video isn't into WimVod"), $testoformat);
                        }
                        $build[$valueKey]["#items"][0][$key_testo] = $testoformat;
                        $build[$valueKey][0]["#markup"] = $testoformat;
                    }
                }
            }
        }
    }
}*/
