#Distill
Distill is a Drupal module that enables other modules to extract and format
data from Drupal entities. It provides a simple class structure for defining
formatting schemas.

##How does Distill Work?
Distill contains 2 classes. 1) A class that defines the structure of a
processor and 2) a distillation class that takes an entity, a processor class,
and a list of fields that should be returned, executes the processor’s
formatter methods, and returns an array of data. If the processor passed into
the distillation class doesn't have a processor for a specific field type,
then it will default to invoking the hook defined for that field type.

###Distill class
`Distill` is a class that takes an entity type, entity, processor, and
language. When asked, it will go through fields, process them with the
methods as defined in the passed-in processor (defaulting to the field type
hooks if no processor function is available), and add them to an array of
field values. It contains a few methods that allow you to ask for fields, and
grab field values.

- `setField('field_name', 'property_name', array('settings'))`: Asks the
distiller to process a given field, and add it to the array of processed fields
with the key as specified in the `property_name` parameter. This function also
takes a settings array, which gets passed into the processor class and allows
one to pass context to the processor function.
- `setAllFields()`: Tells the distiller that all fields should be formatted
and returned.
- `getFieldValues()`: Returns the array of field data that has been extracted
from the given entity and processed by the distiller.

###DistillProcessor class
The `DistillProcessor` class contains methods that provide a sensible default
for extracting and formatting data from fields. The methods are called based
on the field type, or name.

You can create your own processor class that extends `DistillProcessor`, and
easily override processor methods by simply creating methods in the following
pattern:

####Field Type Processor Methods
Field type processor methods are called based on the **type** of field that is
currently being processed. The pattern for creating these method names is
`process*Typename*Type()`, where `*Typename*` is equal to the type of field
this method should process (such as `Text` or `Entityreference`). If your
processor class does not have a method for processing a particular type of
field, then it will invoke `hook_distill_process_*field_type*()`. These
hooks should be implemented by the module defining the field type, or within
the `distill.module` file.


####Field Name Processor Methods
Field name processor methods are called based on the **name** of the field
that's currently being processed. The patter for creating these method names
is `process*Fieldname*Field()`, where `*Fieldname*` is equal to the name of
the field this method should process (such as `Fieldimage` or `Body`).

All processor methods take 3 parameters:

 - `$wrapper`: EntityStructureWrapper|EntityValueWrapper of field from which
 values are being extracted.
 - `$index`: Integer representing the delta of the field being processed.
 - `$settings`: Variable for passing in settings and context that will affect
 how the field value should be processed.

####Example
Here's a quick example implementation of this module within a route controller:

```php
namespace Drupal\my_module\Controller;
use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\JsonResponse;

// Load Distill classes.
use Drupal\distill\Distill;
use Drupal\distill\DistillProcessor;

/**
 * Controller routines for my_module routes.
 */
class DistillExampleController extends ControllerBase {
  /**
   * Returns a blob of yummy json.
   */
  public function exampleDataExtrationEndpoint() {
    $entity = \Drupal::entityManager()->getStorage('node')->load(1);

    // Create instance of processor.
    $processor = new DistillProcessor();

    // Create instance of Distill.
    $distiller = new Distill($entity, $processor);

    // Specify which fields should be returned.
    $distiller->setField('nid', '_id');
    $distiller->setField('title');
    $distiller->setField('body', 'post');

    // We pass in an image style.
    $distiller->setField('field_image', 'image', array('image_style' => 'thumbnail'));

    // This is an entity reference field. We pass a settings array
    // that has an 'include_fields' key. This key contains an array of fields
    // from the referenced entity that should be returned.
    $distiller->setField('field_author', 'user', array(
      'include_fields' => array(
        'name',
        'mail'
      )
    ));

    // Output the returned array as JSON.
    return new JsonResponse($distiller->getFieldValues());
  }
}

```

And here's an example of an entity that's been processed and formatted as JSON:

```javascript
{
  title: "Hello World!",
  post: "<p>This is a post body.</p> ",
  image: "http://d7.local/sites/default/files/styles/thumbnail/field/image/whoa.jpg",
  user: {
    name: "admin",
    mail: "patrickcoffey48@gmail.com"
  }
}
```

## What's Next?
Distill is still under development. Here are some things that are likely to surface soon:

* distill_extra module that contains lots of field-type default formatters.
* Drupal 8 version of Distill.
* RestFull/Distill implementation.
* Better documentation.
* Panels pane that allows developers to easily create and theme panes of content.
