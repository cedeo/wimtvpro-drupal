jQuery.noConflict();

jQuery(document).ready(function() {
    jQuery("a.viewThumbPlaylist, a.viewThumbsPublic").click(function() {
        jQuery(this).colorbox({href: jQuery(this).attr("id")});
    });

    jQuery("a.viewPreviewPlaylist").click(function() {
        jQuery(this).colorbox({
            href: jQuery(this).attr("id"),
            width: '600px', height: '360px'
        });
    });

    function hideAffiliateFields(state) {
        if (state) {
            jQuery(".form-item-companyName").show();
            jQuery(".form-item-affiliateConfirm").show();
        } else {
            jQuery(".form-item-companyName").hide();
            jQuery(".form-item-affiliateConfirm").hide();
        }
    }

    var input_field = jQuery('#edit-affiliate');
    if (jQuery(input_field)) {
        hideAffiliateFields(jQuery(input_field).attr('checked'));
        jQuery(input_field).change(function() {
            hideAffiliateFields(this.checked)
        });
    }

    function wimtvpro_checkCleanUrl(base, url, back) {
        var baseUrl = window.location;
        if (document.location.href.indexOf("?q=") > -1) {
            return "?q=" + base + url;
        } else {
            if (back)
                return back + url;
            else
                return url_pathPlugin + '/' + base + url;
        }
    }

    function callRemoveVideo(element) {
        jQuery.ajax({
            context: this,
            url: wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "wimtvproCallAjax", ""),
            type: "GET",
            dataType: "html",
            async: false,
            data: "namefunction=RemoveVideo&id=" + element.attr("id"),
            beforeSend: function() {
                element.parent().children(".headerBox").children(".icon").hide();
                element.parent().children(".headerBox").children(".loader").show();
            },
            complete: function() {
                element.parent().children(".headerBox").children(".icon").show();
                element.parent().children(".headerBox").children(".loader").hide();
            },
            success: function(response) {
                var json = jQuery.parseJSON(response);
                var result = json.result;
                if (result === "SUCCESS") {
                    element.parent().parent().hide();
                }
                alert(json.result + " : " + json.message);
                location.reload();
            },
            error: function(request, error) {
                alert(request.responseText);
            }
        });
    }

    function callviewVideothumbs(element) {
        var id = element.parent().parent().parent().parent("li").attr("id");
        jQuery(".icon_viewVideo").colorbox({
            html: function() {
                stateView = jQuery(this).attr("rel");

                text = '<p>Do you want view the thumb?</p>';

                text += '<p class="viewThumbs';
                if (stateView == "0")
                    text += " selected";
                text += '" id="0">Invisible</p>';
                text += '<p class="viewThumbs';
                if (stateView == "1")
                    text += " selected";
                text += '" id="1">Only into block "Block list video My Streaming"</p>';
                text += '<p class="viewThumbs';
                if (stateView == "2")
                    text += " selected";
                text += '" id="2">Only into page "My Video Showtime"</p>';
                text += '<p class="viewThumbs';
                if (stateView == "3")
                    text += " selected";
                text += '" id="3">Into block and page</p>';

                text += '<div class="action"><span class="form_save">' + Drupal.t("Save") + '</span><span class="icon_sync2" style="display:none;">Loading...</span></div>';
                return text;
            },
            onComplete: function() {
                jQuery(".viewThumbs").click(function() {
                    jQuery(".viewThumbs").removeClass("selected");
                    jQuery(this).addClass("selected");

                });

                jQuery(".form_save").click(function() {
                    var state = jQuery(".selected").attr("id");

                    //alert (id);
                    jQuery.ajax({
                        context: this,
                        url: wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "wimtvproCallAjax", ""),
                        type: "GET",
                        dataType: "html",
                        data: {
                            state: state,
                            id: id,
                            namefunction: "StateViewThumbs"
                        },
                        beforeSend: function() {
                            jQuery(".icon_sync2").show();
                            jQuery(".form_save").hide();
                        },
                        success: function(response) {
                            jQuery.colorbox.close();
                            element.parent().parent().children(".icon").children("span").attr("rel", state);
                        }
                    });
                });
            }
        });
    }

    function putST(element, namefunction, licenseType, paymentMode, ccType, pricePerView, pricePerViewCurrency, changeClass, coId, id) {
        jQuery.ajax({
            context: this,
            url: wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "wimtvproCallAjax", ""),
            type: "GET",
            dataType: "html",
            data: {
                coId: coId,
                id: id,
                namefunction: namefunction,
                licenseType: licenseType,
                paymentMode: paymentMode,
                ccType: ccType,
                pricePerView: pricePerView,
                pricePerViewCurrency: pricePerViewCurrency
            },
            beforeSend: function() {
                jQuery(".icon_sync2").show();
                jQuery(".form_save").hide();
            },
            success: function(response) {
                var json = jQuery.parseJSON(response);
                var result = json.result;
                if (result === "SUCCESS") {
                    jQuery.colorbox.close();
                    element.closest("td").children("span").hide();
                    element.closest("td").children("span." + changeClass).show();
                    element.closest("td").children("span." + changeClass).attr("id", json.showtimeIdentifier);

                    // NS: WE HIDE DELETE BUTTON
                    element.closest("tr").children(".delete").children("span").hide();

                    // NS: WE CHANGE THE id attribute OF PREVIEW BUTTON TO GET VIDEO FROM WIMVOD.
                    // url = "admin/config/wimtvpro/embedded/" + id + "/" + json.showtimeIdentifier;
                    url = wimtvpro_checkCleanUrl("wimtvpro/embedded/", id + "/" + json.showtimeIdentifier, "")
                    element.closest("tr").children(".view").children("a.viewThumb").show();
                    element.closest("tr").children(".view").children("a.viewThumb").attr("id", url);
                    element.parent().remove();

                } else {
                    jQuery(this).parent().hide();
                    jQuery(this).parent().parent().children(".loader").show();
                    alert(json.messages[0].message);
                    jQuery(".icon_sync2").hide();
                    jQuery(".form_save").show();
                }
            },
            error: function(request, error) {
                alert(request.responseText);
            }
        });
    }

    function callViewForm(element) {
        element.parent().children(".formVideo").fadeToggle("slow");
    }

    function callPutShowtime(element) {
        jQuery(element).colorbox({
            html: function() {
                var thisclass = element.attr("class");
                if (thisclass.indexOf("free") >= 0) {
                    text = "<p>" + Drupal.t("Do you want to publish your videos for free?") + "</p><div class='action'><span class='form_save'><a class='button'>" + Drupal.t("Save") + "</a></span><span class='icon_sync2' style='display:none;'>Loading...</span></div>";
                } else if (thisclass.indexOf("cc") >= 0) {
                    text = '<p class="cc_set" id="BY_NC_SA"><img src="http://www.wim.tv/wimtv-webapp/images/cclicense/Attribution Non-commercial No Derivatives.png" 	title="Attribution Non-Commercial No Derivatives" /> Attribution Non-Commercial No Derivatives</p>';
                    text += '<p class="cc_set" id="BY_NC_ND"><img src="http://www.wim.tv/wimtv-webapp/images/cclicense/Attribution Non-commercial Share Alike.png" 	title="Attribution Non-Commercial Share Alike" /> Attribution Non-Commercial Share Alike</p>';
                    text += '<p class="cc_set" id="BY_NC"><img src="http://www.wim.tv/wimtv-webapp/images/cclicense/Attribution Non-commercial.png" 			title="Attribution Non-Commercial" /> Attribution Non-Commercial</p>';
                    text += '<p class="cc_set" id="BY_ND"><img src="http://www.wim.tv/wimtv-webapp/images/cclicense/Attribution No Derivatives.png" 			title="Attribution No Derivatives" /> Attribution No Derivatives</p>';
                    text += '<p class="cc_set" id="BY_SA"><img src="http://www.wim.tv/wimtv-webapp/images/cclicense/Attribution Share Alike.png" 				title"Attribution Share Alike" /> Attribution Share Alike</p>';
                    text += '<p class="cc_set" id="BY"><img src="http://www.wim.tv/wimtv-webapp/images/cclicense/Attribution.png" 						title="Attribution" /> Attribution</p>';
                    text += '<div class="action"><span class="form_save"><a class="button">' + Drupal.t("Save") + '</a></span><span class="icon_sync2" style="display:none;">Loading...</span></div>';
                } else if (thisclass.indexOf("ppv") >= 0) {
                    text = '<form><input type="text" name="amount" class="amount" value="00" />,<input type="text" name="amount_cent" class="amount_cent" value="00" maxlength="2"/>';
                    text += '<input type="hidden" value="EUR" name="currency" class="currency">Euro';
                    text += '</select></form>';
                    text += '<div class="action"><span class="form_save"><a class="button">' + Drupal.t("Save") + '</a></span><span class="icon_sync2" style="display:none;">Loading...</span></div>';
                }
                return text;
            },
            onComplete: function() {
                jQuery(".cc_set").click(function() {
                    jQuery(".cc_set").removeClass("selected");
                    jQuery(this).addClass("selected");
                });
                jQuery(".form_save").click(function() {
                    var namefunction, licenseType, paymentMode, ccType, pricePerView, pricePerViewCurrency, changeClass, coId, id = "";
                    var id = element.closest("tr").attr("id");
                    var nomeclass = element.parent().siblings("span.add").attr("class");
                    var thisclass = element.attr("class");

                    if (nomeclass === "add icon_Putshowtime") {
                        namefunction = "putST";
                        changeClass = "icon_Removeshowtime";
                    }
                    else if (nomeclass === "add icon_AcquiPutshowtime") {
                        namefunction = "putAcqST";
                        changeClass = "icon_AcqRemoveshowtime";
                        coId = "&acquiredId=" + element.parent().siblings("span.add").attr("id");
                    }
                    if (thisclass.indexOf("free") >= 0) {
                        licenseType = "TEMPLATE_LICENSE";
                        paymentMode = "FREEOFCHARGE";
                    } else if (thisclass.indexOf("cc") >= 0) {
                        licenseType = "CREATIVE_COMMONS";
                        ccType = jQuery(this).parent().parent().children(".selected").attr("id");
                    } else if (thisclass.indexOf("ppv") >= 0) {
                        licenseType = "TEMPLATE_LICENSE";
                        paymentMode = "PAYPERVIEW";
                        pricePerView = jQuery(".amount").val() + "," + jQuery(".amount_cent").val();
                        pricePerViewCurrency = jQuery(".currency").val();
                    }

                    putST(element, namefunction, licenseType, paymentMode, ccType, pricePerView, pricePerViewCurrency, changeClass, coId, id);

                });
            }
        });
    }

    function callRemoveShowtime(element) {
        nomeclass = element.attr("class");
        coId = "";
        if (nomeclass == "icon_AcqRemoveshowtime") {
            namefunction = "removeST";
            changeClass = "icon_AcquiPutshowtime";
            coId = "&showtimeId=" + element.attr("id");
        } else {
            namefunction = "removeST";
            changeClass = "icon_Putshowtime";
            coId = "&showtimeId=" + element.attr("id");
        }
        jQuery.ajax({
            context: this,
            url: wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "wimtvproCallAjax", ""),
            type: "GET",
            dataType: "html",
            data: "namefunction=" + namefunction + "&id=" + element.closest("tr").attr("id") + coId,
            beforeSend: function() {
                //element.parent().hide();
                //element.parent().parent().children(".loader").show();
            },
            complete: function() {
                //element.parent().show();
                //element.parent().parent().children(".loader").hide();
            },
            success: function(response) {
                var json = jQuery.parseJSON(response);
                var result = json.result;
                if (result === "SUCCESS") {
                    showtimeID = element.closest("tr").attr("id");
                    element.hide();
                    element.parent().children("." + changeClass).show();
                    element.parent().children("." + changeClass).attr("id", json.showtimeIdentifier);
                    element.closest("tr").children("td.image").children("div").children("span").children(".icon_licence").hide();
                    if ((nomeclass === "icon_AcquiRemoveshowtime") || (nomeclass === "icon_Removeshowtime")) {
                        element.parent().children(".icon_moveThumbs").hide();
                        element.parent().children(".viewThumb").hide();
                        element.parent().children(".viewThumb").attr("href", "#");
                        element.parent().parent().parent().children("div.infos").hide();

                        // NS: WE CHANGE THE id attribute OF PREVIEW BUTTON TO GET VIDEO FROM WIMBOX.
                        element.closest("tr").children(".view").children("a.viewThumb").show();
                        url = wimtvpro_checkCleanUrl("/admin/config/wimtvpro/embeddedAll/", showtimeID, "");
                        a = element.closest("tr").children(".view").children("a.viewThumb").attr("id", url);

                        // NS: WE SHOW DELETE BUTTON
                        element.closest("tr").children(".delete").children("span").show();

                    } else
                    {
                        element.parent().parent().parent().parent().hide();
                    }
                } else {
                    element.parent().hide();
                    element.parent().parent().children(".loader").show();
                    alert(json.messages[0].message);
                }
            },
            error: function(request, error) {
                alert(request.responseText);
            }
        });
    }

    jQuery(".icon_sync0").click(function() {
        jQuery.ajax({
            context: this,
            url: wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "wimtvproCallSync", ""),
            dataType: "html",
            data: {sync: true, showtime: jQuery("table.items").attr("id")},
            type: "GET",
            beforeSend: function() {
                jQuery(this).removeClass();
                jQuery(this).addClass("icon_sync1");
                jQuery("table.items tbody tr").remove();
            },
            complete: function() {
                jQuery(this).removeClass();
                jQuery(this).addClass("icon_sync0");
            },
            success: function(response) {
//                jQuery("table.items tbody").html(response);
//                jQuery("a.viewThumb").click(function() {
//                    jQuery(this).colorbox({href: jQuery(this).attr("id")});
//                });
//                jQuery("div.wimtv-thumbnail").click(function() {
//                    var url = jQuery(this).parent().parent().children(".view").children("a.viewThumb").attr("id");
//                    jQuery(this).colorbox({href: url});
//                });
//                jQuery(".icon_Putshowtime,.icon_AcquiPutshowtime").click(function() {
//                    callViewForm(jQuery(this));
//                });
//                jQuery(".icon_AcqRemoveshowtime,.icon_Removeshowtime,.icon_RemoveshowtimeInto").click(function() {
//                    callRemoveShowtime(jQuery(this));
//                });
//                jQuery(".free,.cc,.pay").click(function() {
//                    callPutShowtime(jQuery(this));
//                });
//                jQuery(".icon_remove").click(function() {
//                    callRemoveVideo(jQuery(this));
//                });
                location.reload();

                //callviewVideothumbs(jQuery(this));
            },
            error: function(response) {
                jQuery("ul.items").html(response);
            }
        });
    });

    jQuery(".free,.cc,.ppv").click(function() {
        callPutShowtime(jQuery(this));
    });

    jQuery(".icon_Putshowtime,.icon_AcquiPutshowtime").click(function() {
        callViewForm(jQuery(this));
    });

    jQuery(".icon_AcqRemoveshowtime,.icon_Removeshowtime,.icon_RemoveshowtimeInto").click(function() {
        callRemoveShowtime(jQuery(this));
    });

    jQuery(".icon_viewVideo").click(function() {
        callviewVideothumbs(jQuery(this));
    });

    jQuery(".icon_remove").click(function() {
        title = jQuery(this).closest("tr").children("td").children(".title").html();
        if (confirm(Drupal.t("You are removing video with title:\n\n\t'" + title + "'\n\nAre you sure ?")))
        {
            callRemoveVideo(jQuery(this));
        }
    });

    jQuery('#edit-sandbox').change(function() {
        if (jQuery(this).attr('value') == "no") {
            jQuery('#sandbox').attr('href', 'http://www.wim.tv/wimtv-webapp/userRegistration.do?execution=e1s1');
            jQuery('#site').html('www.wim.tv');
            jQuery('input[value="basePathWimtv"]').attr('value', 'https://www.wim.tv/wimtv-webapp/rest/');
        } else {
            jQuery('#sandbox').attr('href', 'http://www.wim.tv/wimtv-webapp/userRegistration.do?execution=e1s1');
            jQuery('#site').html('peer.wim.tv');
            jQuery('input[value="basePathWimtv"]').attr('value', 'http://peer.wim.tv/wimtv-webapp/rest/');
        }
    });

    jQuery(".icon_download").click(function() {
        var id = jQuery(this).attr("id");
        console.log("Downloading video " + id + "...");
        downloadVideo(id);
    });

    function downloadVideo(contentid) {
        var uri = wimtvpro_checkCleanUrl("admin/config/wimtvpro/", "download/" + contentid);
        jQuery("body").append("<iframe  style=\"display:none\" id=\"iframeDownload\" src=\"" + uri + "\" />");
    }

});

function detectAllShortCodes(textbody) {
    jQuery(".addThumb").each(function(i, val) {
        detectShortCode(textbody, val.id);
    });
}

function detectShortCode(textbody, cleanShortCode) {
    if (textbody.match(cleanShortCode))
    {
        jQuery("#" + cleanShortCode).parent().addClass("select");
    }
}