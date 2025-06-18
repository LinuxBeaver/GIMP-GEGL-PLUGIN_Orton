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
 * Credit to Øyvind Kolås (pippin) for major GEGL contributions
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

property_enum(type, _("Type of Orton"),
    ortons, ortonkind,
    ORIGINAL)
  description(_("Choose what type of orton you want"))

enum_start (ortonkind)
enum_value   (ORIGINAL, "original", N_("Original Orton theory"))
enum_value   (JUNE2025REVISE, "june2025revision", N_("Revised Orton"))
enum_value   (DARKTHEORY, "darkorton", N_("Dark Orton"))
enum_value   (COLORTHEORY, "colororton", N_("Color Orton"))
  enum_end (ortons)

property_double (sharpen_layer_opacity, _("Opacity of the sharpen layer"), 1.0)
   description(_("(all but original) Opacity of the layer doing  the sharpen effect"))
   value_range (0.0, 1.0)
    ui_meta ("visible", "type {june2025revision, darkorton, colororton}")



property_double (sharpen, _("Sharpen original image below orton"), 0.0)
   description (_("(original only) The GEGL graph contains two layers one sharp one blured and blended. This slider controls the sharpness of the original image. In default sharpen is at 0 which means there is no sharpen unless the slider is moved up"))
   value_range (0.0, 2.0)
   ui_range    (0.0, 2.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
    ui_meta ("visible", "!type {june2025revision, darkorton, colororton}")

property_double (sharpen2, _("Sharpen"), 3.0)
   description (_("(all but original) Sharpen the virtual layer above"))
   value_range (0.0, 10.0)
   ui_range    (0.0, 10.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
    ui_meta ("visible", "type {june2025revision, darkorton, colororton}")



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
   description (_("(original only) Blend mode's are most noticable when the gaussian blur opacity is high"))
    blendortonbeaver, blend_orton_beaver,
    GEGL_BLEND_MODE_TYPE_NORMAL)
    ui_meta ("visible", "!type {june2025revision, darkorton, colororton}")

property_double (std_dev_x, _("Gaussian blur horizontal"), 7.0)
   description (_("(original only) Horizontal standard deviation for the horizontal axis"))
   value_range (0.0, 100.0)
   ui_range    (0.24, 30.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")
    ui_meta ("visible", "!type {june2025revision, darkorton, colororton}")

property_double (std_dev_y, _("Gaussian blur vertical"), 7.0)
   description (_("(original only) Vertical standard deviation (spatial scale factor)"))
   value_range (0.0, 100.0)
   ui_range    (0.24, 30.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "y")
    ui_meta ("visible", "!type {june2025revision, darkorton, colororton}")

property_double (std_dev_x2, _("Gaussian blur horizontal"), 14.0)
   description (_("(all but original) Horizontal standard deviation for the horizontal axis"))
   value_range (0.0, 100.0)
   ui_range    (0.24, 30.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "x")
    ui_meta ("visible", "type {june2025revision, darkorton, colororton}")

property_double (std_dev_y2, _("Gaussian blur vertical"), 14.0)
   description (_("(all but original) Vertical standard deviation (spatial scale factor)"))
   value_range (0.0, 100.0)
   ui_range    (0.24, 30.0)
   ui_gamma    (3.0)
   ui_meta     ("unit", "pixel-distance")
   ui_meta     ("axis", "y")
    ui_meta ("visible", "type {june2025revision, darkorton, colororton}")



property_double (opacity, _("Gaussian blur opacity"), 0.6)
   description(_("(all excluding revision) Opacity of the overlayed gaussian blur"))
   value_range (0.0, 0.9)
   ui_range    (0.3, 0.9)
    ui_meta ("visible", "!type {june2025revision}")

property_double (opacity2, _("Gaussian blur opacity"), 0.9)
   description(_("(revision only) Opacity of the overlayed gaussian blur"))
   value_range (0.0, 0.9)
   ui_range    (0.3, 0.9)
    ui_meta ("visible", "type {june2025revision}")



property_double (dark, _("Dark value"), 1.8)
   description(_("(original and revised) Make the orton darker"))
  value_range (1.0, 3.0)
    ui_meta ("visible", "!type {darkorton, colororton}")

property_double (light, _("Light value"), 2.0)
   description(_("(original) Make the orton lighter"))
   value_range (1.0, 3.0)
    ui_meta ("visible", "!type {june2025revision, darkorton, colororton}")


property_double (light2, _("Light value"), 1.2)
   description(_("(all excluding original) Make the orton lighter"))
   value_range (1.0, 1.5)
    ui_meta ("visible", "type {june2025revision, darkorton, colororton}")

property_double (luma, _("Luminance Darken"), -25.0)
   description(_("(dark orton only) Luminance darkening of the orton"))
   value_range (-40, 0.0)
    ui_meta ("visible", "!type {june2025revision, original, colororton}")


property_double (saturation, _("Saturation control"), 1.0)
   description(_("(original only) Control the orton's saturation"))
   value_range (0.0, 1.0)
    ui_meta ("visible", "!type {june2025revision, darkorton, colororton}")

property_double (saturation2, _("Saturation control"), 1.0)
   description(_("(revision only) Control the orton's saturation"))
   value_range (0.0, 1.0)
    ui_meta ("visible", "type {june2025revision}")

property_double (hardlight_highout, _("Hardlight High output blend"), 1.0)
   description(_("(dark orton only) Levels high output blended with the hardlight blend mode. At 0 it is entirely removed. "))
   value_range (0.0, 1.0)
    ui_meta ("visible", "type {darkorton}")


enum_start (dark_orton_beaver)
  enum_value (GEGL_KEEP_COLOR, "color",
              N_("Keep Color"))
  enum_value (GEGL_LUMINANCE,      "luminance",
              N_("Luminance Desaturation"))
  enum_value (GEGL_LUMA,      "luma",
              N_("Luma Desaturation"))
  enum_value (GEGL_LIGHTNES_HSL,      "lighness",
              N_("Lightness Desaturation"))
  enum_value (GEGL_AVERAGE_HSI,      "average",
              N_("Average Desaturation"))
/*
  enum_value (GEGL_VALUE_HSV,      "value",
              N_("Value Desaturation")) */
enum_end (darkortonbeaver)

property_enum (desaturation_setting, _("Desaturation setting"),
   description (_(" (dark orton only) Rather full desaturation is enabled and if so what type of desaturation do you want?"))
    darkortonbeaver, dark_orton_beaver,
    GEGL_KEEP_COLOR)
    ui_meta ("visible", "type {darkorton}")


enum_start (blend_color_orton_beaver)
  enum_value (GEGL_SOFTLIGHT, "softlight",
              N_("Softlight"))
  enum_value (GEGL_OVERLAY,      "overlay",
              N_("Overlay"))
  enum_value (GEGL_BURN,      "burn",
              N_("Burn"))
  enum_value (GEGL_LCHCOLOR,      "lchcolor",
              N_("LCH Color"))
  enum_value (GEGL_HSLCOLOR,      "hslcolor",
              N_("HSL Color"))
enum_end (blendcolorortonbeaver)

property_enum (color_blend, _("Blend Color"),
   description (_("(color only) Blend the color"))
    blendcolorortonbeaver, blend_color_orton_beaver,
    GEGL_OVERLAY)
    ui_meta ("visible", "type {colororton}")

property_double (color_opacity, _("Color Opacity"), 0.8)
   description(_("(color only) Control the opacity of the color overlay"))
   value_range (0.0, 1.0)
    ui_meta ("visible", "type {colororton}")



property_color (color, _("Color"), "#fff100")
    description (_("(color only) The color to paint over the orton"))
    ui_meta ("visible", "type {colororton}")


property_boolean (smooth_band, _("Smooth bandings?"), FALSE)
    description  (_("(dark orton only) smooth the dark orton's bandings, it will be very slow"))
    ui_meta ("visible", "type {darkorton}")

#else

#define GEGL_OP_META
#define GEGL_OP_NAME     orton
#define GEGL_OP_C_SOURCE orton.c

#include "gegl-op.h"

/*starred nodes go inside typedef struct */

typedef struct
{
 GeglNode *input;
 GeglNode *input2;
 GeglNode *blur;
 GeglNode *over;
 GeglNode *over2;
 GeglNode *idref;
 GeglNode *multiply;
 GeglNode *screen;
 GeglNode *hardlight;
 GeglNode *darken;
 GeglNode *lighten;
 GeglNode *gamma;
 GeglNode *opacity;
 GeglNode *opacity2;
 GeglNode *opacity3;
 GeglNode *opacity4;
 GeglNode *opacity5;
 GeglNode *opacity6;
 GeglNode *saturation;
 GeglNode *sharpen;
 GeglNode *output;
 GeglNode *output2;
 GeglNode *over_re;
 GeglNode *gamma_re;
 GeglNode *multiply_re;
 GeglNode *sharpen_re;
 GeglNode *blur_re;
 GeglNode *opacity_re;
 GeglNode *saturation_re;
 GeglNode *saturation_dark;
 GeglNode *fix;
 GeglNode *fix2;
 GeglNode *luma;
 GeglNode *light;
 GeglNode *gray;
 GeglNode *keepcolor;
 GeglNode *grayscale1;
 GeglNode *grayscale2;
 GeglNode *grayscale3;
 GeglNode *grayscale4;
 GeglNode *grayscale5;
 GeglNode *color;
 GeglNode *softlight;
 GeglNode *overlay;
 GeglNode *burn;
 GeglNode *lchcolor;
 GeglNode *hslcolor;
 GeglNode *idrefdark;
 GeglNode *idrefdark2;
 GeglNode *hardlightdark;
 GeglNode *levelsdark;
 GeglNode *denoise;
 GeglNode *dontdenoise;

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

  state->input2    = gegl_node_get_input_proxy (gegl, "input");
  state->output2   = gegl_node_get_output_proxy (gegl, "output");

 state->blur = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur", "abyss-policy", TRUE, NULL);

 state->over = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);

 state->over2 = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);


 state->idref = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);


 state->multiply = gegl_node_new_child (gegl, "operation", "gegl:multiply", NULL);

 state->hardlight = gegl_node_new_child (gegl, "operation", "gegl:hard-light", "srgb", TRUE, NULL);

 state->lighten = gegl_node_new_child (gegl, "operation", "gegl:lighten", NULL);

 state->darken = gegl_node_new_child (gegl, "operation", "gegl:darken", NULL);

 state->screen = gegl_node_new_child (gegl, "operation", "gegl:screen", NULL);

 state->gamma = gegl_node_new_child (gegl, "operation", "gegl:gamma", NULL);

 state->sharpen = gegl_node_new_child (gegl, "operation", "gegl:unsharp-mask", NULL);




 state->saturation = gegl_node_new_child (gegl, "operation", "gegl:saturation", NULL);

 state->opacity = gegl_node_new_child (gegl, "operation", "gegl:opacity", NULL);

 state->opacity2 = gegl_node_new_child (gegl, "operation", "gegl:opacity",  NULL);

 state->opacity3 = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 0.7,  NULL);

 state->opacity4 = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 0.4,  NULL);

 state->opacity5 = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 1.15,  NULL);

 state->opacity6 = gegl_node_new_child (gegl, "operation", "gegl:opacity", "value", 1.3,  NULL);

 state->multiply_re = gegl_node_new_child (gegl, "operation", "gegl:multiply", NULL);

 state->saturation_re = gegl_node_new_child (gegl, "operation", "gegl:saturation", NULL);

 state->saturation_dark = gegl_node_new_child (gegl, "operation", "gegl:saturation", "scale", 0.0, NULL);

 state->opacity_re = gegl_node_new_child (gegl, "operation", "gegl:opacity", NULL);

 state->blur_re = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur", "abyss-policy", TRUE, NULL);

 state->over_re = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);

 state->blur_re = gegl_node_new_child (gegl, "operation", "gegl:gaussian-blur", "abyss-policy", TRUE, NULL);

 state->over_re = gegl_node_new_child (gegl, "operation", "gegl:over", NULL);

 state->gamma_re = gegl_node_new_child (gegl, "operation", "gegl:gamma", NULL);

 state->sharpen_re = gegl_node_new_child (gegl, "operation", "gegl:unsharp-mask", NULL);

 state->light = gegl_node_new_child (gegl, "operation", "gegl:hue-chroma", NULL);

 state->luma = gegl_node_new_child (gegl, "operation", "gimp:layer-mode", "layer-mode", 56, NULL);

 state->idrefdark = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

 state->fix = gegl_node_new_child (gegl, "operation", "gegl:median-blur", "radius", 0, "abyss-policy", 0, NULL);

 state->fix2 = gegl_node_new_child (gegl, "operation", "gegl:median-blur", "radius", 0, "abyss-policy", 0, NULL);

 state->gray = gegl_node_new_child (gegl, "operation", "gegl:gray", NULL);

 state->keepcolor = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

 state->grayscale1 = gegl_node_new_child (gegl, "operation", "gimp:desaturate", "mode", 0, NULL);

 state->grayscale2 = gegl_node_new_child (gegl, "operation", "gimp:desaturate", "mode", 1, NULL);

 state->grayscale3 = gegl_node_new_child (gegl, "operation", "gimp:desaturate", "mode", 2, NULL);

 state->grayscale4 = gegl_node_new_child (gegl, "operation", "gimp:desaturate", "mode", 3, NULL);

 state->grayscale5 = gegl_node_new_child (gegl, "operation", "gimp:desaturate", "mode", 4, NULL);

 state->color = gegl_node_new_child (gegl, "operation", "gegl:color", NULL);

 state->softlight = gegl_node_new_child (gegl, "operation", "gimp:layer-mode", "layer-mode", 35,  "composite-mode", 0, NULL);

 state->burn = gegl_node_new_child (gegl, "operation", "gimp:layer-mode", "layer-mode", 43,  "composite-mode", 0, NULL);

 state->overlay = gegl_node_new_child (gegl, "operation", "gimp:layer-mode", "layer-mode", 23,  "composite-mode", 0, NULL);

state->lchcolor = gegl_node_new_child (gegl,
                                    "operation", "gimp:layer-mode", "layer-mode", 26, "composite-mode", 0,  "blend-space", 3, NULL);

state->hslcolor = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 39, "composite-mode", 0, NULL);

state->hardlightdark = gegl_node_new_child (gegl,
                                  "operation", "gimp:layer-mode", "layer-mode", 44, "composite-mode", 0, NULL);

 state->idrefdark2 = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

 state->levelsdark = gegl_node_new_child (gegl, "operation", "gegl:multiply", "value", 0.5, NULL);

 state->denoise = gegl_node_new_child (gegl, "operation", "gegl:denoise-dct", "sigma", 4.0, NULL);

 state->dontdenoise = gegl_node_new_child (gegl, "operation", "gegl:nop", NULL);

/*meta redirect property to new child orders go here

 gegl_operation_meta_redirect (operation, "propertyname", state->newchildname,  "originalpropertyname");
*/


}

static void
update_graph (GeglOperation *operation)
{
  GeglProperties *o = GEGL_PROPERTIES (operation);
  State *state = o->user_data;
  if (!state) return;
  GeglNode *smooth = state->dontdenoise;

/*
  gegl_node_disconnect(state->over2, "input");
  gegl_node_disconnect(state->over2, "aux");

gegl_node_disconnect(state->saturation, "input");
gegl_node_disconnect(state->saturation, "output");
gegl_node_disconnect(state->blur, "input");
gegl_node_disconnect(state->blur, "output");
gegl_node_disconnect(state->opacity, "input");
gegl_node_disconnect(state->opacity, "output");

  gegl_node_link_many (state->input,  state->blur, state->opacity, state->multiply,  state->gamma, state->saturation,  NULL);
 gegl_node_disconnect(state->sharpen, "input");
 gegl_node_disconnect(state->sharpen, "output");
gegl_node_disconnect(state->multiply, "input");
gegl_node_disconnect(state->multiply, "output");
gegl_node_disconnect(state->gamma, "input");
gegl_node_disconnect(state->gamma, "output");
*/



  GeglNode *over = state->over; /* the default */
  switch (o->blendmode) {
    case GEGL_BLEND_MODE_TYPE_NORMAL: over = state->over; break;
    case GEGL_BLEND_MODE_TYPE_SCREEN: over = state->screen; break;
    case GEGL_BLEND_MODE_TYPE_DARKEN: over = state->darken; break;
    case GEGL_BLEND_MODE_TYPE_LIGHTEN: over = state->lighten; break;
    case GEGL_BLEND_MODE_TYPE_HARDLIGHT: over = state->hardlight; break;

default: over = state->over;
}

  GeglNode *decolor = state->keepcolor; /* the default */
  switch (o->desaturation_setting) {
    case GEGL_KEEP_COLOR: decolor = state->keepcolor; break;
    case GEGL_LUMINANCE: decolor = state->grayscale1; break;
    case GEGL_LUMA: decolor = state->grayscale2; break;
    case GEGL_LIGHTNES_HSL: decolor = state->grayscale3; break;
    case GEGL_AVERAGE_HSI: decolor = state->grayscale4; break;
 /*   case GEGL_VALUE_HSV: decolor = state->grayscale5; break;*/

default: decolor = state->keepcolor;

}

  GeglNode *blendcolor = state->softlight; /* the default */
  switch (o->color_blend) {
    case GEGL_SOFTLIGHT: blendcolor = state->softlight; break;
    case GEGL_OVERLAY: blendcolor = state->overlay; break;
    case GEGL_BURN: blendcolor = state->burn; break;
    case GEGL_LCHCOLOR: blendcolor = state->lchcolor; break;
    case GEGL_HSLCOLOR: blendcolor = state->hslcolor; break;


default: blendcolor = state->overlay;




}

  if (o->smooth_band) smooth  = state->denoise;
  if (!o->smooth_band) smooth  = state->dontdenoise;
/*optional connect from and too is here
  gegl_node_connect (state->blendmode, "aux", state->lastnodeinlist, "output"); */
switch (o->type) { 
       break;
    case ORIGINAL:
  gegl_node_link_many (state->input, state->sharpen, over,  state->output,  NULL);
gegl_node_link_many (state->input, state->blur, state->opacity, state->multiply, state->gamma, state->saturation, NULL);
  gegl_node_connect (over, "aux", state->saturation, "output");
         break;
    case JUNE2025REVISE:
  gegl_node_link_many (state->input, state->over, state->idref, state->over2, state->output,  NULL);
  gegl_node_connect (state->over, "aux", state->saturation_re, "output");
  gegl_node_link_many (state->input,  state->blur_re, state->opacity_re, state->multiply_re,  state->gamma_re, state->saturation_re,  NULL);
  gegl_node_connect (state->over2, "aux", state->opacity3, "output");
  gegl_node_link_many (state->idref,  state->sharpen_re, state->opacity2, state->opacity3,  NULL);
 break;
    case DARKTHEORY:
  gegl_node_link_many (state->input, decolor, state->over, state->idref, state->over2,  state->idrefdark2, state->hardlightdark, smooth,  state->output,  NULL);
  gegl_node_connect (state->over, "aux", state->luma, "output");
  gegl_node_link_many (state->input,  state->blur_re, state->opacity, state->opacity5, state->multiply_re,  state->saturation_dark, state->fix, state->idrefdark, state->luma,   NULL);
  gegl_node_connect (state->over2, "aux", state->gray, "output");
  gegl_node_link_many (state->idref,  state->sharpen_re, state->opacity2, state->opacity4, state->gray,  NULL);
  gegl_node_connect (state->luma, "aux", state->light, "output");
  gegl_node_link_many (state->idrefdark, state->light,  NULL);
  gegl_node_connect (state->hardlightdark, "aux", state->levelsdark, "output");
  gegl_node_link_many (state->idrefdark2, state->levelsdark,  NULL);
 break;
    case COLORTHEORY:
  gegl_node_link_many (state->input, state->grayscale1, state->over, state->idref, state->over2,  state->fix2, blendcolor, state->output,  NULL);
  gegl_node_connect (state->over, "aux", state->fix, "output");
  gegl_node_link_many (state->input,  state->blur_re, state->opacity, state->opacity6, state->multiply_re,  state->saturation_dark, state->fix,   NULL);
  gegl_node_connect (state->over2, "aux", state->gray, "output");
  gegl_node_link_many (state->idref,  state->sharpen_re, state->opacity2, state->opacity4, state->gray,  NULL);
  gegl_node_connect (blendcolor, "aux", state->color, "output");
 break;
}

 gegl_operation_meta_redirect (operation, "dark", state->gamma,  "value");
 gegl_operation_meta_redirect (operation, "light", state->multiply,  "value");
 gegl_operation_meta_redirect (operation, "opacity", state->opacity,  "value");
 gegl_operation_meta_redirect (operation, "std-dev-x", state->blur,  "std-dev-x");
 gegl_operation_meta_redirect (operation, "std-dev-y", state->blur,  "std-dev-y");
 gegl_operation_meta_redirect (operation, "saturation", state->saturation,  "scale");
 gegl_operation_meta_redirect (operation, "sharpen", state->sharpen,  "scale");

 gegl_operation_meta_redirect (operation, "sharpen_layer_opacity", state->opacity2,  "value");


 gegl_operation_meta_redirect (operation, "dark", state->gamma_re,  "value");
 gegl_operation_meta_redirect (operation, "light2", state->multiply_re,  "value");
 gegl_operation_meta_redirect (operation, "std-dev-x", state->blur_re,  "std-dev-x");
 gegl_operation_meta_redirect (operation, "std-dev-y", state->blur_re,  "std-dev-y");
 gegl_operation_meta_redirect (operation, "saturation2", state->saturation_re,  "scale");
 gegl_operation_meta_redirect (operation, "std-dev-x2", state->blur_re,  "std-dev-x");
 gegl_operation_meta_redirect (operation, "std-dev-y2", state->blur_re,  "std-dev-y");
 gegl_operation_meta_redirect (operation, "sharpen2", state->sharpen_re,  "scale");
 gegl_operation_meta_redirect (operation, "opacity2", state->opacity_re,  "value");

 gegl_operation_meta_redirect (operation, "luma", state->light,  "lightness");

 gegl_operation_meta_redirect (operation, "color", state->color,  "value");

 gegl_operation_meta_redirect (operation, "color_opacity", blendcolor,  "opacity");

 gegl_operation_meta_redirect (operation, "hardlight_highout", state->hardlightdark,  "opacity");

 }  





static void
gegl_op_class_init (GeglOpClass *klass)
{

  GeglOperationClass     *operation_class      = GEGL_OPERATION_CLASS (klass);
  GeglOperationMetaClass *operation_meta_class = GEGL_OPERATION_META_CLASS (klass);

  operation_class->attach      = attach;
  operation_meta_class->update = update_graph;


  gegl_operation_class_set_keys (operation_class,
    "name",        "lb:orton",
    "title",       _("Orton"),
    "reference-hash", "germancockroach",
    "description", _("Orton effect"),
/*<Image>/Colors <Image>/Filters are top level menus in GIMP*/
    "gimp:menu-label", _("Orton Effect..."),
    NULL);
}

#endif
