/**
 * Created by wistd on 2026/06/10.
 * Copyright (c) 2026 All rights reserved.
 */

#ifndef WIT_WIT_H
#define WIT_WIT_H

namespace wit {

struct WitSoundPlayerTag;

typedef WitSoundPlayerTag* WitSoundPlayerHn;

#ifdef __cplusplus
    extern "C" {
#endif

    /**
     * TODO: function here.
     */

    /**
     *
     */
	bool					WitInitialize				(															);

    /**
     *
     */
    void					WitUninitialize				(															);

    /**
     *
     * @return
     */
    WitSoundPlayerHn		WitCreateSoundPlayer		(															);

    /**
     *
     * @param handle
     */
    void					WitDestroySoundPlayer		(WitSoundPlayerHn handle									);

    /**
     *
     * @param handle
     * @param cue_name
     */
    void					WitSetCueSoundPlayer		(WitSoundPlayerHn handle, const char* cue_name				);

    /**
     *
     * @param handle
     */
    void					WitPlaySound				(WitSoundPlayerHn handle									);

    /**
     *
     * @param player
     */
    void					WitPauseSound				(WitSoundPlayerHn player									);

    /**
     *
     */
    void					WitPauseAllSound			(															);

    /**
     *
     */
    void					WitUpdateMainThread			(															);

#ifdef __cplusplus
    }
#endif
}

#endif //WIT_WIT_H
