<?php
/**
 * @file
 * Template file for the overview form.
 */
  $count = 0;
?>

<div id="fields-overview">

  <table id="fields" class="sticky-enabled">
    <thead>
      <tr>
        <th><?php print t('name'); ?></th>
        <th><?php print t('Actions'); ?></th>
        <th><?php print t('Weight'); ?></th>
      </tr>
    </thead>

    <tbody>
    <?php foreach ($rows as $row): ?>

      <tr class="<?php print $count % 2 == 0 ? 'odd' : 'even'; ?> draggable property-row">
        <td><?php print $row->title; ?></td>
        <td><?php print l(t('View'), 'admin/dashboard/'. $row->did) .' | '. l(t('Edit'), 'admin/dashboard/configure/'. $row->did .'/edit') .' | '. l(t('Delete'), 'admin/dashboard/configure/'. $row->did .'/delete'); ?></td>
        <td><?php print $row->weight; ?></td>
      </tr>

    <?php endforeach; ?>
    </tbody>
  </table>

  <?php print $submit; ?>

</div>
