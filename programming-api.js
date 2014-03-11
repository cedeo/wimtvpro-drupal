/**
 * Created with JetBrains PhpStorm.
 * User: walter
 * Date: 24/02/14
 * Time: 14.25
 * To change this template use File | Settings | File Templates.
 */
/**
 * require('programming.js')
 */

ProgUtils.api = {};

ProgUtils.api.getBaseUrl = function (){
    return url_pathPlugin;
};

/*** For images ***/
ProgUtils.api.getBaseImageUrl = function (){
    return imageBase + "/";
};
/*** GET ***/

/**
 * API relativa agli item di un palinsesto in un dato periodo di tempo
 *
 * @param: progId	il programming identifier di riferimento
 */
function buildUrl(base, path) {
    if (base.indexOf("?") != -1) {
        return base + "&" + path;
    } else {
        return base + "?" + path;
    }
}

ProgUtils.api.calendar = function(progId) {
    return buildUrl(programmingBase, "api=calendar&progId=" + progId);
};

/**
 * Torna HTML del pool di video da usare come base
 * per i vari “giorni” del calendario in cui si crea una programmazione
 */
ProgUtils.api.pool = function() {
    return buildUrl(programmingBase, "api=pool");
};

/**
 * Torna HTML del pool di video da usare come base
 * per i vari “giorni” del calendario in cui si crea una programmazione
 */
ProgUtils.api.itemsAt = function() {
    return buildUrl(programmingBase, "api=currentProgramming");
};

/*** POST ***/

/**
 * Aggiunge/modifica informazioni generali palinsesto (es.nome)
 */
ProgUtils.api.programmingInfo = function() {
    return buildUrl(programmingBase, "api=programmings");
};

/**
 * Aggiunge Item al palinsesto in un dato momento
 *
 * @param: progId	il programming identifier di riferimento
 */
ProgUtils.api.addItem = function(progId) {
    return buildUrl(programmingBase, "api=addItem&progId=" + progId);
};


/*** DELETE ***/

/**
 * rimuove eventi dal palinsesto,
 * corrispondente ad un giorno solare nel calendario
 *
 * @param: progId	il programming identifier di riferimento
 *
 * JQUERY BUG in DELETE
 * non appende i parametri in data su query string
 */
ProgUtils.api.deleteItems = function(progId,itemId) {
    return buildUrl(programmingBase, "api=removeItem&progId=" + progId + '&itemId='+ itemId + "&");
};

/**
 * Elimina Item dal palinsesto
 *
 * @param: progId	il programming identifier di riferimento
 * @param: itemId	ref. a item da eliminare
 */
ProgUtils.api.removeItem = function(progId, itemId) {
    return buildUrl(programmingBase, "api=deleteItems&progId=" + progId + '&itemId='+ itemId);
};

/**
 * Update di un Item esistente sul palinsesto in un dato momento
 *
 * @param: progId	il programming identifier di riferimento
 * @param: itemId	ref. a item da aggiornare
 */
ProgUtils.api.updateItem = function(progId, itemId) {
    return buildUrl(programmingBase, "api=updateItem&progId=" + progId + '&itemId='+ itemId);
};

/**
 * Copia tutti gli elementi di un item in una o pià date di destinazione
 *
 * @param: progId	il programming identifier di riferimento
 */
ProgUtils.api.mimicItem = function(progId) {
    return buildUrl(programmingBase, "api=mimicItem&progId=" + progId);
};