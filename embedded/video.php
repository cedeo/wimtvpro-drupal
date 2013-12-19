<div style='text-align:center;'>
    <?php echo $response ?>
    <h3><?php echo $title ?></h3>
    <p><?php echo $description ?></p>
    <p>Duration: <b><?php echo $duration ?></b>
        <br/>Categories<br/>
        <?php foreach ($categories as $idx => $category) { ?>
            <i><?php echo $category->categoryName ?>:</i>
            <?php foreach ($category->subCategories as $key => $subcategory) {
                echo $subcategory->categoryName;
                if ($key < count($category->subCategories)-1)
                    echo ", ";
            } ?>
        <br/>
        <?php } ?>
    </p>
</div>