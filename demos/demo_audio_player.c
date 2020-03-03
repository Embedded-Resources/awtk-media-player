/**
 * File:   demo_media_player.c
 * Author: AWTK Develop Team
 * Brief:  media_player demo
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-05-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "awtk.h"
#include "media_player/widgets/lrc_view.h"
#include "media_player/base/media_player.h"
#include "media_player/audio_player/audio_decoder_mp3.h"
#include "media_player/audio_player/audio_decoder_factory.h"
#include "media_player/audio_player/media_player_audio.h"

static ret_t on_media_player_event(void* ctx, event_t* e) {
  switch (e->type) {
    case EVT_MEDIA_PLAYER_LOADED: {
      media_player_loaded_event_t* evt = media_player_loaded_event_cast(e);
      log_debug("w=%u h=%u duration=%u\n", evt->video_width, evt->video_height, evt->duration);
      break;
    }
    case EVT_MEDIA_PLAYER_PAUSED: {
      log_debug("paused\n");
      break;
    }
    case EVT_MEDIA_PLAYER_DONE: {
      log_debug("done\n");
      break;
    }
  }
  return RET_OK;
}

static ret_t on_load_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_load(player, "./data/test.mp3");

  return RET_OK;
}

static ret_t on_forward_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;
  uint32_t position = media_player_get_position(player) + 3000;

  media_player_seek(player, position);

  return RET_OK;
}

static ret_t on_start_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_start(player);

  return RET_OK;
}

static ret_t on_stop_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_stop(player);

  return RET_OK;
}

static ret_t on_pause_click(void* ctx, event_t* e) {
  media_player_t* player = (media_player_t*)ctx;

  media_player_pause(player);

  return RET_OK;
}

static ret_t on_quit_click(void* ctx, event_t* e) {
  tk_quit();
  return RET_OK;
}

static ret_t app_global_init(void) {
  lrc_view_register();
  media_player_set(media_player_audio_create());
  data_reader_factory_set(data_reader_factory_create());
  data_reader_factory_register(data_reader_factory(), "file", data_reader_file_create);
  audio_decoder_factory_set(audio_decoder_factory_create());
  audio_decoder_factory_register(audio_decoder_factory(), "mp3", audio_decoder_mp3_create);

  return RET_OK;
}

static ret_t application_init() {
  media_player_t* player = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);
  widget_t* lrc_View = lrc_view_create(win, 20, 20, win->w - 40, win->h - 40);
  widget_t* load = button_create(win, 0, 0, 0, 0);
  widget_t* start = button_create(win, 0, 0, 0, 0);
  widget_t* forward = button_create(win, 0, 0, 0, 0);
  widget_t* pause = button_create(win, 0, 0, 0, 0);
  widget_t* stop = button_create(win, 0, 0, 0, 0);
  widget_t* quit = button_create(win, 0, 0, 0, 0);
  widget_t* status = label_create(win, 0, 0, 0, 0);
  
  app_global_init();
  player = media_player();
  media_player_set_on_event(player, on_media_player_event, player);

  widget_set_prop_str(win, WIDGET_PROP_THEME, "audio_player");
  widget_set_self_layout_params(status, "center", "m:-120", "100%", "30");

  widget_set_text(load, L"load");
  widget_set_self_layout_params(load, "center", "middle:-80", "50%", "30");
  widget_on(load, EVT_CLICK, on_load_click, player);

  widget_set_text(start, L"start");
  widget_set_self_layout_params(start, "center", "middle:-40", "50%", "30");
  widget_on(start, EVT_CLICK, on_start_click, player);

  widget_set_text(forward, L"forward");
  widget_set_self_layout_params(forward, "center", "middle", "50%", "30");
  widget_on(forward, EVT_CLICK, on_forward_click, player);

  widget_set_text(pause, L"pause");
  widget_set_self_layout_params(pause, "center", "middle:40", "50%", "30");
  widget_on(pause, EVT_CLICK, on_pause_click, player);

  widget_set_text(stop, L"stop");
  widget_set_self_layout_params(stop, "center", "m:80", "50%", "30");
  widget_on(stop, EVT_CLICK, on_stop_click, player);

  widget_set_text(quit, L"Quit");
  widget_set_self_layout_params(quit, "center", "m:120", "50%", "30");
  widget_on(quit, EVT_CLICK, on_quit_click, quit);

  widget_layout(win);

  return RET_OK;
}

ret_t application_exit() {
  data_reader_factory_destroy(data_reader_factory());

  return RET_OK;
}

#define LCD_WIDTH 800
#define LCD_HEGHT 480
#include "awtk_main.inc"
