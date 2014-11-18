jQuery(document).ready(function() {
//  SHOW JQUERY VERSION  - alert(jQuery.fn.jquery);
    jQuery.noConflict();

    var textbody = jQuery(field_to_insert).val();
    detectAllShortCodes(textbody);

    jQuery.fn.extend({
        insertAtCaret: function(valueToInsertAtCaret) {
            return this.each(function(i) {
                if (document.selection) {
                    this.focus();
                    selection = document.selection.createRange();
                    selection.text = valueToInsertAtCaret;
                    this.focus();
                } else if (this.selectionStart || this.selectionStart == "0") {
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

    var text = jQuery(field_to_insert).val();
    var n = null;
    if (text !== "")
        n = text.match(/\[wimtv](.*?)\[\/wimtv\]/g);

    if (n !== null) {
        jQuery.each(n, function(i, val) {
            val = val.replace("[wimtv]", "");
            val = val.replace("[/wimtv]", "");
            array = val.split("|");
            jQuery("#" + array[0]).parent().parent().parent().addClass("select");
            jQuery("#" + array[0]).parent().parent().children(".w").attr("disabled", "disabled");
            jQuery("#" + array[0]).parent().parent().children(".h").attr("disabled", "disabled");
            jQuery("#" + array[0]).parent().parent().children(".w").val(array[1]);
            jQuery("#" + array[0]).parent().parent().children(".h").val(array[2]);
        });
    }

    try {
        jQuery(".wimtv-thumbnail").colorbox({});
    }
    catch (err)
    {
    }

    jQuery("a.addThumb").click(function() {
        var text = "[wimtv]" + jQuery(this).attr("id") + "|" + jQuery(this).parent().children("p").children(".w").val() + "|" + jQuery(this).parent().children("p").children(".h").val() + "[/wimtv]";
        jQuery(this).parent().children("p").children(".w").attr("disabled", "disabled");
        jQuery(this).parent().children("p").children(".h").attr("disabled", "disabled");
        jQuery(field_to_insert).insertAtCaret(text);
        jQuery(this).parent().addClass("select");
    });

    jQuery("a.removeThumb").click(function() {
        var testo = jQuery("#edit-body-und-0-value").val() + "";
        testo = testo.replace("[wimtv]" + jQuery(this).attr("id") + "|" + jQuery(this).parent().children("p").children(".w").val() + "|" + jQuery(this).parent().children("p").children(".h").val() + "[/wimtv]", "");
        jQuery(this).parent().children("p").children(".w").removeAttr("disabled");
        jQuery(this).parent().children("p").children(".h").removeAttr("disabled");

        jQuery(field_to_insert).val(testo);
        jQuery(this).parent().removeClass("select");
    });
});