<?php

/**
 * @file
 * Settings form template file.
 */

print $above_widget_settings;

if ($rows): ?>
<div id="field-content">
  <table id="fields" class="sticky-enabled">
    <thead>
      <tr>
        <th><?php print t('Widget'); ?></th>
        <th><?php print t('Region'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>
    <tbody>

    <!-- Node regions -->
    <?php foreach ($regions as $region => $title): ?>
      <tr class="region region-<?php print $region?>">
        <td colspan="3" class="region"><?php print $title; ?></td>
      </tr>
      <tr class="region-message region-<?php print $region?>-message <?php print empty($rows[$region]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="3"><em><?php print t('No widgets in this region'); ?></em></td>
      </tr>

      <!-- fields -->
      <?php
      if (!empty($rows[$region])):
        $count = 0;
        foreach ($rows[$region] as $row):?>
          <tr class="<?php print $count % 2 == 0 ? 'odd' : 'even'; ?> <?php print $row->class ?>">
            <td><?php print $row->title; ?></td>
            <td><?php print $row->region; ?></td>
            <td><?php print $row->weight; ?></td>
          </tr>
          <?php
          $count++;
        endforeach;
      endif;
    endforeach;
    ?>
    </tbody>
  </table>
</div>
<?php
print $below_widget_settings;

print $submit;
endif;
