jQuery(document).ready(function() {
    jQuery.noConflict();
    changeTitle();
    jQuery("form").submit(function() {

        if (jQuery("#wimtv_fileuploaded").val() != "") {
            alert("Attention: You still have to upload the video");
            jQuery("#wimtv_fileuploaded").addClass(" required error ");
            return false;
        } else {
            return true;
        }

    });
    jQuery(".field_upload").click(function() {
        //ajaxFileUpload(this);
    });


    // MERGING FILE
    var file_field = jQuery("input#wimtv_fileuploaded");
    var id = file_field.attr("id");
    var name = file_field.attr("name");
    // var url = wimtvpro_checkCleanUrl("", "wimtvpro/wimtvproCallUpload", "../../");
    var url = wimtvpro_checkCleanUrl(drupalRootPath + "/admin/config/wimtvpro/", "wimtvproCallUpload", "");
    jQuery("#wimtv_fileuploaded").fileupload({
        dataType: "json",
        type: "GET",
        url: url,
        multipart: true,
        done: function(e, data, jqXHR) {
            var response = data.jqXHR.responseText;
            var elencoVideo = jQuery(".videosId");
            response = jQuery.parseJSON(response);
            if (response.error4 !== "") {
                alert(response.msg);
            } else {
                //Add row into #view_video_add
                var titleVideo = response.titleVideo;
                titleVideo = titleVideo.replace("\'", "");
                var row = "<tr>";
                if (response.urlThumbs === "") {
                    response.urlThumbs = "<div class=\'none\'></div>";
                }
                row += "<td class=\'video\'>" + response.urlThumbs + "</td>"; //urlVideo
                row += "<td class=\'titlevideo\'><input class=\'title\' type=\'text\' value=\'" + titleVideo + "\'/><span class=\'icon_modTitleVideo\' rel=\'" + response.vid + "\'></span><strong class=\'icon_savemodTitleVideo\' rel=\'" + response.vid + "\'>Apply</strong></td>";
                row += "<td id=\'" + response.vid + "\'><a class=\'icon_remove\' id=\'" + response.contentId + "\' onClick=\'removeVideo(this)\'></a></td>";
                row += "</tr>";

                jQuery("#view_video_add").append(row);
                jQuery(".field_upload").show();
                var val = elencoVideo.val();
                if (val !== "") {
                    val += "," + response.vid;
                }
                else {
                    val = response.vid;
                }
                elencoVideo.val(val);
                jQuery("#wimtv_fileuploaded").val("");
                changeTitle();

            }
            jQuery("#progress .bar").attr("style", "width:0");
            jQuery("#progress .percent").html("");
            data.jqXHR.abort();
        },
        fail: function(e, data) {
            alert("Upload failed!");
        },
        add: function(e, data) {
            var title_field = jQuery(".fileName_text").val();
            data.fileInput = jQuery(this);
            data.formData = {"name": jQuery(this).attr("name"), "filetitle": title_field, "id": id};
            data.submit();
            jQuery(".field_upload").hide();
        },
        progressall: function(e, data) {
            var progress = parseInt(data.loaded / data.total * 100, 10);
            jQuery("#progress .bar").css(
                    "width",
                    progress + "%"
                    )

            jQuery("#progress .percent").html(progress + "%");
        },
        processfail: function(e, data) {
            alert(data);
        }
    });
});

function removeVideo(element) {
//    var elencoVideo = jQuery(".videosId");
    jQuery.ajax({
        context: this,
        url: wimtvpro_checkCleanUrl("", "admin/config/wimtvpro/wimtvproCallAjax", "../../"),
        type: "GET",
        dataType: "html",
        async: false,
        data: "namefunction=RemoveVideo&id=" + jQuery(element).attr("id"),
        beforeSend: function() {
            jQuery(".icon_save").hide();
            jQuery("#progressbar").hide();
        },
        complete: function() {
        },
        success: function(response) {

            var json = jQuery.parseJSON(response);
            var result = json.result;
            alert(json.result + ": " + json.message);

            if (result === "SUCCESS") {
//                var elencoVideo = jQuery(".videosId");
//                jQuery(element).parent().parent().remove();
//                var vid = jQuery(element).parent().attr("id");
//                var val = elencoVideo.val();
//                val = val.replace(vid, "");
//                val = val.replace(",,", "");
//                elencoVideo.val(val);
                removeVideoFromContent(element);
            }
            jQuery(".icon_save").show();
            jQuery("#progressbar").hide();
        },
        error: function(request, error) {
            alert(request.responseText);
            jQuery(".icon_save").show();
        }
    })

}

function removeVideoFromContent(element) {
    var elencoVideo = jQuery(".videosId");
    jQuery(element).parent().parent().remove();
    var vid = jQuery(element).parent().attr("id");
    var val = elencoVideo.val();
    val = val.replace(vid, "");
    val = val.replace(",,", "");
    elencoVideo.val(val);
}

function ajaxFileUpload(obj) {

    var file_field = jQuery(obj).parent().parent("div").children("div").children("input.form-file");

    wimtvpro_title(jQuery(obj).parent().parent("div").children("div").children("input.form-text"));
    var title_field = jQuery(obj).parent().parent("div").children("div").children("input.form-text").val();
    title_field = title_field.replace("\'", "");
    var elencoVideo = jQuery(obj).parent().parent("div").children(".videosId");
    var bar = 0;
    //jQuery("#progressbar").progressbar({value: 0});
    //jQuery("#progressbar #percent").html("");
    if (file_field.val() != "") {

        //jQuery("#progressbar").progressbar({value: 0});
        //jQuery(".field_upload").hide();
        //jQuery("#progressbar").show();

        //jQuery(".icon_throbber").show();
        var id = file_field.attr("id");
        var name = file_field.attr("name");



        /*
         jQuery.ajaxFileUpload({
         
         url: wimtvpro_checkCleanUrl("","wimtvpro/wimtvproCallUpload","../../"),
         secureuri:false,
         fileElementId:id,
         fileElementName:"fileUploadWimTv",
         
         dataType: "json",
         data:{name:"fileUploadWimTv",filetitle:title_field , id:id},
         
         beforeSend:function(){			
         jQuery(".icon_throbber").show();
         },
         
         success: function (data, status){
         
         if(data.error4 != ""){
         alert(data.error4);
         jQuery("#progressbar").progressbar({value:0});
         jQuery(".field_upload").show();
         //jQuery("#progressbar").hide();
         jQuery(".icon_throbber").hide();
         } else {
         //Add row into #view_video_add
         jQuery(".icon_throbber").hide();
         var titleVideo = data.titleVideo;
         titleVideo = titleVideo.replace("\'","");
         var row = "<tr>";
         if (data.urlThumbs=="") data.urlThumbs = "<div class=\'none\'></div>";
         row += "<td class=\'video\'>" + data.urlThumbs + "</td>"; //urlVideo
         row += "<td class=\'titlevideo\'><input class=\'title\' type=\'text\' value=\'" +  titleVideo + "\'/><span class=\'icon_modTitleVideo\' rel=\'" + data.vid +  "\'></span><strong class=\'icon_savemodTitleVideo\' rel=\'" + data.vid +  "\'>Apply</strong></td>";
         row += "<td id=\'" + data.vid + "\'><a class=\'icon_remove\' id=\'"+ data.contentId +"\' onClick=\'removeVideo(this)\'></a></td>";
         row += "</tr>";
         
         jQuery("#view_video_add").append(row);
         jQuery("#progressbar").progressbar({value:100});
         jQuery("#progressbar").hide();
         jQuery(".field_upload").show();
         var val = elencoVideo.val();
         if (val!="") val += "," + data.vid;
         else val=data.vid;
         elencoVideo.val(val);
         jQuery("#wimtv_fileuploaded").val("");
         
         }
         
         changeTitle();
         },
         
         
         error: function (data, status, e){
         jQuery(".field_upload").show();
         jQuery("#progressbar").hide();
         alert("error2:" + e + "-" + status + "-" + data);}
         });
         */

    } else {
        alert("Please select a file and try again.");
    }

    return false;
}

function wimtvpro_checkCleanUrl(base, url, back) {
    var baseUrl = window.location;
    if (document.location.href.indexOf("?q=") > -1) {
        return "?q=" + base + url;
    } else {
        if (back)
            return back + url;
        else
            return base + url;
    }
}

function wimtvpro_title(obj) {

    if (obj.val() == "") {
        var title = jQuery("#edit-title").val();

        obj.val(title.replace("\'", ""));

    }

}

function wimtvpro_TestFileType(obj) {
//    fileName = obj.val();
    fileName = obj.val().split('\\').pop();
    fileTypes = ["", "mov", "mpg", "avi", "flv", "mpeg", "mp4", "mkv", "m4v"];
    if (!fileName) {
        return;
    }

    dots = fileName.split(".");
    // get the part AFTER the LAST period.
    fileType = "." + dots[dots.length - 1];

    if (fileTypes.join(".").indexOf(fileType.toLowerCase()) !== -1) {
        return true;
    } else {
        alert("Please only upload files that end in types: \n\n" + (fileTypes.join(" .")) + "\n\nPlease select a new file and try again.");
        obj.val("");
    }

}

function changeTitle() {
    jQuery(".titlevideo .title").click(function() {
        jQuery(this).parent().children(".icon_savemodTitleVideo").show();
        jQuery(this).parent().children(".icon_modTitleVideo").hide();
    });
    jQuery(".titlevideo .icon_modTitleVideo").click(function() {
        jQuery(this).parent().children(".icon_savemodTitleVideo").show();
        jQuery(this).parent().children(".icon_modTitleVideo").hide();
        jQuery(this).parent().children(".title").addClass("focus");
    });

    jQuery(".icon_savemodTitleVideo").click(function() {
        var titleVideo = jQuery(this).parent().children(".title").val();



        jQuery.ajax({
            context: this,
            url: wimtvpro_checkCleanUrl("", "admin/config/wimtvpro/wimtvproCallAjax", "../../"),
            type: "GET",
            dataType: "html",
            async: false,
            data: "namefunction=ModifyTitleVideo&titleVideo=" + titleVideo + "&id=" + jQuery(this).attr("rel"),
            complete: function() {
            },
            success: function(response) {

                jQuery(this).hide();
                jQuery(this).parent().children(".icon_modTitleVideo").show();
                alert("Update successful");
                jQuery(this).parent().children(".title").removeClass("focus");
            },
            error: function(request, error) {
                alert(request.responseText);
                jQuery(this).hide();
                jQuery(this).parent().children(".icon_modTitleVideo").show();
                jQuery(this).parent().children(".title").removeClass("focus");
            }
        })


    });
}
