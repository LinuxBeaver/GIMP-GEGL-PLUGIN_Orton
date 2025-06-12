/* This file is an image processing operation for GEGL
 *
 * GEGL is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * GEGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GEGL; if not, see <https://www.gnu.org/licenses/>.
 *
 * Credit to Øvind Kolas (pippin) for major GEGL contributions
 * 2024, beaver, Orton

id=1 over aux=[ ref=1 gaussian-blur std-dev-x=10 std-dev-y=10
abyss-policy=none
opacity value=0.7 multiply value=2  gamma value=2  ]

]

end of syntax
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#ifdef GEGL_PROPERTIES

/*Properties go here*/

property_double (sharpen, _("Sharpen original image below orton"), 0.0)
   description (_("The GEGL graph contains two layers one sharp one blured and blended. This slider controls the sharpness of the original image. In default sharpen is at 0 which means there is no sharpen unless the slider is moved up"))
   value_range (0.0, 2.0)
   ui_range    (0.0, 2.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")



enum_start (blend_orton_beaver)
  enum_value (GEGL_BLEND_MODE_TYPE_NORMAL, "normal",
              N_("Normal"))
  enum_value (GEGL_BLEND_MODE_TYPE_SCREEN,      "screen",
              N_("Screen"))
  enum_value (GEGL_BLEND_MODE_TYPE_DARKEN,      "darken",
              N_("Darken"))
  enum_value (GEGL_BLEND_MODE_TYPE_LIGHTEN,      "lighten",
              N_("Lighten"))
  enum_value (GEGL_BLEND_MODE_TYPE_HARDLIGHT,      "hardlight",
              N_("Hard Light"))
enum_end (blendortonbeaver)

property_enum (blendmode, _("Blend Mode of Orton"),
   description (_("Blend mode's are most noticable when the gaussian blur opacity is high"))
    blendortonbeaver, blend_orton_beaver,
    GEGL_BLEND_MODE_TYPE_NORMAL)


property_double (std_dev_x, _("Gaussian blur horizontal"), 7.0)
   description (_("Horizontal standard deviation for the horizontal axis"))
   value_range (0.0, 100.0)
   ui_range    (0.24, 30.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")

property_double (std_dev_y, _("Gaussian blur vertical"), 7.0)
   description (_("Vertical standard deviation (spatial scale factor)"))
   value_range (0.0, 100.0)
   ui_range    (0.24, 30.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "y")


property_double (opacity, _("Gaussian blur opacity"), 0.6)
   description(_("Opacity of the overlayed gaussian blur"))
   value_range (0.0, 0.9)
   ui_range    (0.3, 0.9)

property_double (dark, _("Dark value"), 1.8)
   description(_("Make the orton darker"))
  value_range (1.0, 3.0)

property_double (light, _("Light value"), 2.0)
   description(_("Make the orton lighter"))
   value_range (1.0, 3.0)

property_double (saturation, _("Saturation control"), 1.0)
   description(_("Control the orton's saturation"))
   value_range (0.0, 1.0)

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     orton
#define GEGL_OP_C_SOURCE orton.c

#include "gegl-op.h"

/*starred nodes go inside typedef struct */

typedef struct
{
 GeglNode *input;
 GeglNode *blur;
 GeglNode *over;
 GeglNode *multiply;
 GeglNode *screen;
 GeglNode *hardlight;
 GeglNode *darken;
 GeglNode *lighten;
 GeglNode *gamma;
 GeglNode *opacity;
 GeglNode *saturation;
 GeglNode *sharpen;
 GeglNode *output;
}State;

static void attach (GeglOperation *operation)
{
  GeglNode *gegl = operation->node;
  GeglProperties *o = GEGL_PROPERTIES (operation);


  State *state = o->user_data = g_malloc0 (sizeof (State));

/*new child node list is here, this is where starred nodes get defined

 state->newchildname = gegl_node_new_child (gegl, "operation", "lb:name", NULL);*/
  state->input    = gegl_node_get_input_proxy (gegl, "input");
  state->output   = gegl_node_get_output_proxy (gegl, "output");

 state->blur = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur", "abyss-policy", TRUE, NULL);

 state->over = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);

 state->multiply = gegl_node_new_child (gegl, "operation", "gegl:multiply", NULL);

 state->hardlight = gegl_node_new_child (gegl, "operation", "gegl:hard-light", "srgb", TRUE, NULL);

 state->lighten = gegl_node_new_child (gegl, "operation", "gegl:lighten", NULL);

 state->darken = gegl_node_new_child (gegl, "operation", "gegl:darken", NULL);

 state->screen = gegl_node_new_child (gegl, "operation", "gegl:screen", NULL);

 state->gamma = gegl_node_new_child (gegl, "operation", "gegl:gamma", NULL);

 state->sharpen = gegl_node_new_child (gegl, "operation", "gegl:unsharp-mask", NULL);

 state->saturation = gegl_node_new_child (gegl, "operation", "gegl:saturation", NULL);

 state->opacity = gegl_node_new_child (gegl, "operation", "gegl:opacity", NULL);

/*meta redirect property to new child orders go here

 gegl_operation_meta_redirect (operation, "propertyname", state->newchildname,  "originalpropertyname");
*/

 gegl_operation_meta_redirect (operation, "dark", state->gamma,  "value");
 gegl_operation_meta_redirect (operation, "light", state->multiply,  "value");
 gegl_operation_meta_redirect (operation, "opacity", state->opacity,  "value");
 gegl_operation_meta_redirect (operation, "std-dev-x", state->blur,  "std-dev-x");
 gegl_operation_meta_redirect (operation, "std-dev-y", state->blur,  "std-dev-y");
 gegl_operation_meta_redirect (operation, "saturation", state->saturation,  "scale");
 gegl_operation_meta_redirect (operation, "sharpen", state->sharpen,  "scale");
}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;

  GeglNode *over = state->over; /* the default */
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_NORMAL: over = state->over; break;
    case GEGL_BLEND_MODE_TYPE_SCREEN: over = state->screen; break;
    case GEGL_BLEND_MODE_TYPE_DARKEN: over = state->darken; break;
    case GEGL_BLEND_MODE_TYPE_LIGHTEN: over = state->lighten; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: over = state->hardlight; break;

default: over = state->over;

}

  gegl_node_link_many (state->input, state->sharpen, over,  state->output,  NULL);
  gegl_node_connect (over, "aux", state->saturation, "output");
  gegl_node_link_many (state->input,  state->blur, state->opacity, state->multiply,  state->gamma, state->saturation,  NULL);


/*optional connect from and too is here
  gegl_node_connect (state->blendmode, "aux", state->lastnodeinlist, "output"); */

}

static void
gegl_op_class_init (GeglOpClass *klass)
{
  GeglOperationClass *operation_class;
GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);
  operation_class = GEGL_OPERATION_CLASS (klass);

  operation_class->attach = attach;
  operation_meta_class->update = update_graph;

  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:orton",
    "title",       _("Orton"),
    "reference-hash", "germancockroach",
    "description", _("Orton effect"),
/*<Image>/Colors <Image>/Filters are top level menus in GIMP*/
    "gimp:menu-path", "<Image>/Filters/Artistic",
    "gimp:menu-label", _("Orton Effect..."),
    NULL);
}

#endif
