<div style='text-align:center;'>
    <?php
    // NS: HERE WE RENDER THE PLAYER
    echo $response
    ?>
    <h3><?php echo $title ?></h3>
    <p><?php echo $description ?></p>
    <p><?php echo t("Duration"); ?>: <b><?php echo $duration ?></b>
        <br/><?php echo t("Category-Subcategory"); ?><br/>
        <?php foreach ($categories as $idx => $category) { ?>
            <i><?php echo $category->categoryName ?>:</i>
            <?php
            foreach ($category->subCategories as $key => $subcategory) {
                echo $subcategory->categoryName;
                if ($key < count($category->subCategories) - 1)
                    echo ", ";
            }
            ?>
            <br/>
        <?php } ?>
    </p>
</div>