#ifndef INPUT_H_
# define INPUT_H_

# include <SFML/Graphics.h>
# include <SFML/System.h>

# include "animation/sora.h"
# include "graphic/sprite.h"
# include "message/message.h"
# include "std.h"

typedef struct s_input input;

struct            s_input
{
  sfVideoMode     video_mode;
  sfRenderWindow  *render_window;
  sfEvent         event;

  sora_animation  sora;
  sprite_graphic  anim;
};

struct s_input_class
{
  void (*constructor)(input *this);
  void (*destructor)(input *this);

  void (*update)(input *this, sfTime datetime, list *objects);
  void (*init)(input *this);
  void (*send_message)(input *this, message *msg);
};

extern struct s_input_class g_input;

#endif /* !INPUT_H_ */
