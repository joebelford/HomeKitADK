/*
 * App_Camera.c
 *
 *  Created on: Dec 29, 2020
 *      Author: joe
 */

#include "HAP.h"
#include "HAPTLV+Internal.h"
#include "HAPBase.h"

#include "App_Camera.h"
#include "App.h"
#include "DB.h"

bool isValid(void* unsused HAP_UNUSED) {
    return true;
}

supportedAudioConfigStruct supportedAudioConfigValue =
{
    .audioCodecConfig =
    {
        .audioCodecType = 2,     // AAC-ELD
        .audioCodecParams =
        {
            .audioChannels = 1,  // 1 channel
            .bitRate = 0,        // Variable
            .sampleRate = 0      // 8kHz
        }
    },
    .comfortNoiseSupport = false
};

HAP_STRUCT_TLV_SUPPORT(void, SupportedAudioConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, AudioCodecConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, AudioCodecParamsFormat)

const HAPUInt8TLVFormat audioCodecParamsAudioChannelsFormat = { .type = kHAPTLVFormatType_UInt8,
                                                                .constraints = { .minimumValue = 1, .maximumValue = 1 },
                                                                .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember audioCodecParamsAudioChannelsMember = {
    .valueOffset = HAP_OFFSETOF(audioCodecParamsStruct, audioChannels),
    .isSetOffset = 0,
    .tlvType = 1,
    .debugDescription = "Audio Codec Config Params Audio Channels",
    .format = &audioCodecParamsAudioChannelsFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat audioCodecParamsBitRateFormat = { .type = kHAPTLVFormatType_UInt8,
                                                          .constraints = { .minimumValue = 0, .maximumValue = 2 },
                                                          .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember audioCodecParamsBitRateMember = { .valueOffset = HAP_OFFSETOF(audioCodecParamsStruct, bitRate),
                                                           .isSetOffset = 0,
                                                           .tlvType = 2,
                                                           .debugDescription = "Audio Codec Config Params Bit-Rate",
                                                           .format = &audioCodecParamsBitRateFormat,
                                                           .isOptional = false,
                                                           .isFlat = false };

const HAPUInt8TLVFormat audioCodecParamsSampleRateFormat = { .type = kHAPTLVFormatType_UInt8,
                                                             .constraints = { .minimumValue = 0, .maximumValue = 2 },
                                                             .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember audioCodecParamsSampleRateMember = { .valueOffset =
                                                                      HAP_OFFSETOF(audioCodecParamsStruct, sampleRate),
                                                              .isSetOffset = 0,
                                                              .tlvType = 3,
                                                              .debugDescription =
                                                                      "Audio Codec Config Params Sample Rate",
                                                              .format = &audioCodecParamsSampleRateFormat,
                                                              .isOptional = false,
                                                              .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const HAPUInt16TLVFormat audioCodecTypeFormat = { .type = kHAPTLVFormatType_UInt16,
                                                  .constraints = { .minimumValue = 0, .maximumValue = 6 },
                                                  .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember audioCodecTypeMember = { .valueOffset = HAP_OFFSETOF(audioCodecConfigStruct, audioCodecType),
                                                  .isSetOffset = 0,
                                                  .tlvType = 1,
                                                  .debugDescription = "Audio Codec Type",
                                                  .format = &audioCodecTypeFormat,
                                                  .isOptional = false,
                                                  .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const AudioCodecConfigFormat audioCodecParamsFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &audioCodecParamsAudioChannelsMember,
                                                     &audioCodecParamsBitRateMember,
                                                     &audioCodecParamsSampleRateMember,
                                                     NULL },
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember audioCodecParamsMember = { .valueOffset =
                                                            HAP_OFFSETOF(audioCodecConfigStruct, audioCodecParams),
                                                    .isSetOffset = 0,
                                                    .tlvType = 2,
                                                    .debugDescription = "Audio Codec Parameters",
                                                    .format = &audioCodecParamsFormat,
                                                    .isOptional = false,
                                                    .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const AudioCodecConfigFormat audioCodecConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &audioCodecTypeMember, &audioCodecParamsMember, NULL },
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember audioCodecConfigMember = { .valueOffset =
                                                            HAP_OFFSETOF(supportedAudioConfigStruct, audioCodecConfig),
                                                    .isSetOffset = 0,
                                                    .tlvType = 1,
                                                    .debugDescription = "Audio Codec Config",
                                                    .format = &audioCodecConfigFormat,
                                                    .isOptional = false,
                                                    .isFlat = false };

const HAPUInt8TLVFormat comfortNoiseFormat = { .type = kHAPTLVFormatType_UInt8,
                                               .constraints = { .minimumValue = 0, .maximumValue = 255 },
                                               .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember comfortNoiseMember = { .valueOffset =
                                                        HAP_OFFSETOF(supportedAudioConfigStruct, comfortNoiseSupport),
                                                .isSetOffset = 0,
                                                .tlvType = 2,
                                                .debugDescription = "Comfort Noise Support",
                                                .format = &comfortNoiseFormat,
                                                .isOptional = false,
                                                .isFlat = false };

const SupportedAudioConfigFormat supportedAudioConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &audioCodecConfigMember, &comfortNoiseMember, NULL },
    .callbacks = { .isValid = isValid }
};

supportedVideoConfigStruct supportedVideoConfigValue = {
    .videoCodecConfig = { .videoCodecType = 0,
                          .videoCodecParams = { .profileID = 1,
                                                .level = 2,
                                                .packetizationMode = 0,
                                                .CVOEnabled = 0 }, // TODO - Make enums for profileID, and level
                          .videoAttributes = { .imageWidth = 1920, .imageHeight = 1080, .frameRate = 30 } }
};

HAP_STRUCT_TLV_SUPPORT(void, SupportedVideoConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, VideoCodecConfigFormat)
HAP_STRUCT_TLV_SUPPORT(void, VideoCodecParamsFormat)
HAP_STRUCT_TLV_SUPPORT(void, VideoAttributesFormat)

const HAPUInt8TLVFormat videoCodecParamsProfileIDFormat = { .type = kHAPTLVFormatType_UInt8,
                                                            .constraints = { .minimumValue = 0, .maximumValue = 2 },
                                                            .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoCodecParamsProfileIDMember = { .valueOffset =
                                                                     HAP_OFFSETOF(videoCodecParamsStruct, profileID),
                                                             .isSetOffset = 0,
                                                             .tlvType = 1,
                                                             .debugDescription = "Video Codec Config Params Profile ID",
                                                             .format = &videoCodecParamsProfileIDFormat,
                                                             .isOptional = false,
                                                             .isFlat = false };

const HAPUInt8TLVFormat videoCodecParamsLevelFormat = { .type = kHAPTLVFormatType_UInt8,
                                                        .constraints = { .minimumValue = 0, .maximumValue = 2 },
                                                        .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoCodecParamsLevelMember = { .valueOffset = HAP_OFFSETOF(videoCodecParamsStruct, level),
                                                         .isSetOffset = 0,
                                                         .tlvType = 2,
                                                         .debugDescription = "Video Codec Config Params Level",
                                                         .format = &videoCodecParamsLevelFormat,
                                                         .isOptional = false,
                                                         .isFlat = false };

const HAPUInt8TLVFormat videoCodecParamsPacketizationModeFormat = { .type = kHAPTLVFormatType_UInt8,
                                                                    .constraints = { .minimumValue = 0,
                                                                                     .maximumValue = 2 },
                                                                    .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoCodecParamsPacketizationModeMember = {
    .valueOffset = HAP_OFFSETOF(videoCodecParamsStruct, packetizationMode),
    .isSetOffset = 0,
    .tlvType = 3,
    .debugDescription = "Video Codec Config Packetization Mode",
    .format = &videoCodecParamsPacketizationModeFormat,
    .isOptional = false,
    .isFlat = false
};

const HAPUInt8TLVFormat videoCodecParamsCVOEnabledFormat = { .type = kHAPTLVFormatType_UInt8,
                                                             .constraints = { .minimumValue = 0, .maximumValue = 2 },
                                                             .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoCodecParamsCVOEnabledMember = { .valueOffset =
                                                                      HAP_OFFSETOF(videoCodecParamsStruct, CVOEnabled),
                                                              .isSetOffset = 0,
                                                              .tlvType = 4,
                                                              .debugDescription = "CVO Enabled",
                                                              .format = &videoCodecParamsCVOEnabledFormat,
                                                              .isOptional = false,
                                                              .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const HAPUInt8TLVFormat videoCodecTypeFormat = { .type = kHAPTLVFormatType_UInt8,
                                                 .constraints = { .minimumValue = 0, .maximumValue = 1 },
                                                 .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoCodecTypeMember = { .valueOffset = HAP_OFFSETOF(videoCodecConfigStruct, videoCodecType),
                                                  .isSetOffset = 0,
                                                  .tlvType = 1,
                                                  .debugDescription = "Video Codec Type",
                                                  .format = &videoCodecTypeFormat,
                                                  .isOptional = false,
                                                  .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const VideoCodecConfigFormat videoCodecParamsFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoCodecParamsProfileIDMember,
                                                     &videoCodecParamsLevelMember,
                                                     &videoCodecParamsPacketizationModeMember,
                                                     &videoCodecParamsCVOEnabledMember,
                                                     NULL },
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember videoCodecParamsMember = { .valueOffset =
                                                            HAP_OFFSETOF(videoCodecConfigStruct, videoCodecParams),
                                                    .isSetOffset = 0,
                                                    .tlvType = 2,
                                                    .debugDescription = "Video Codec Parameters",
                                                    .format = &videoCodecParamsFormat,
                                                    .isOptional = false,
                                                    .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const HAPUInt16TLVFormat videoAttributesImageWidthFormat = { .type = kHAPTLVFormatType_UInt16,
                                                             .constraints = { .minimumValue = 0, .maximumValue = 4096 },
                                                             .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoAttributesImageWidthMember = { .valueOffset =
                                                                     HAP_OFFSETOF(videoAttributesStruct, imageWidth),
                                                             .isSetOffset = 0,
                                                             .tlvType = 1,
                                                             .debugDescription = "Video Attributes Image Width",
                                                             .format = &videoAttributesImageWidthFormat,
                                                             .isOptional = false,
                                                             .isFlat = false };

const HAPUInt16TLVFormat videoAttributesImageHeightFormat = { .type = kHAPTLVFormatType_UInt16,
                                                              .constraints = { .minimumValue = 0,
                                                                               .maximumValue = 4096 },
                                                              .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoAttributesImageHeightMember = { .valueOffset =
                                                                      HAP_OFFSETOF(videoAttributesStruct, imageHeight),
                                                              .isSetOffset = 0,
                                                              .tlvType = 2,
                                                              .debugDescription = "Video Attributes Image Height",
                                                              .format = &videoAttributesImageHeightFormat,
                                                              .isOptional = false,
                                                              .isFlat = false };

const HAPUInt8TLVFormat videoAttributesFrameRateFormat = { .type = kHAPTLVFormatType_UInt8,
                                                           .constraints = { .minimumValue = 0, .maximumValue = 255 },
                                                           .callbacks = { .getDescription = NULL } };
const HAPStructTLVMember videoAttributesFrameRateMember = { .valueOffset =
                                                                    HAP_OFFSETOF(videoAttributesStruct, frameRate),
                                                            .isSetOffset = 0,
                                                            .tlvType = 3,
                                                            .debugDescription = "Video Attributes Frame Rate",
                                                            .format = &videoAttributesFrameRateFormat,
                                                            .isOptional = false,
                                                            .isFlat = false };

const VideoAttributesFormat videoAttributesFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoAttributesImageWidthMember,
                                                     &videoAttributesImageHeightMember,
                                                     &videoAttributesFrameRateMember,
                                                     NULL },
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember videoAttributesMember = { .valueOffset = HAP_OFFSETOF(videoCodecConfigStruct, videoAttributes),
                                                   .isSetOffset = 0,
                                                   .tlvType = 3,
                                                   .debugDescription = "Video Attributes",
                                                   .format = &videoAttributesFormat,
                                                   .isOptional = false,
                                                   .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const VideoCodecConfigFormat videoCodecConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoCodecTypeMember,
                                                     &videoCodecParamsMember,
                                                     &videoAttributesMember,
                                                     NULL },
    .callbacks = { .isValid = isValid }
};
const HAPStructTLVMember videoCodecConfigMember = { .valueOffset =
                                                            HAP_OFFSETOF(supportedVideoConfigStruct, videoCodecConfig),
                                                    .isSetOffset = 0,
                                                    .tlvType = 1,
                                                    .debugDescription = "Video Codec Config",
                                                    .format = &videoCodecConfigFormat,
                                                    .isOptional = false,
                                                    .isFlat = false };

/* ---------------------------------------------------------------------------------------------*/

const SupportedVideoConfigFormat supportedVideoConfigFormat = {
    .type = kHAPTLVFormatType_Struct,
    .members = (const HAPStructTLVMember* const[]) { &videoCodecConfigMember, NULL },
    .callbacks = { .isValid = isValid }
};

HAP_RESULT_USE_CHECK
HAPError VerifyCodecConfigTLV(void* actualBytes, size_t numActualBytes) {
    HAPError err;
    HAPTLVReaderRef configReader;
    HAPTLVReaderCreateWithOptions(
            &configReader,
            &(const HAPTLVReaderOptions) {
                    .bytes = actualBytes, .maxBytes = numActualBytes, .numBytes = numActualBytes });
    HAPTLV tlv;
    bool found;
    err = HAPTLVReaderGetNext(&configReader, &found, &tlv);
    HAPAssert(!err);
    if (tlv.type == 1) {
        uint8_t codecBuffer[tlv.value.numBytes];
        HAPRawBufferCopyBytes(codecBuffer, &tlv.value.bytes[0], tlv.value.numBytes);
        HAPTLVReaderRef codecReader;
        HAPTLVReaderCreateWithOptions(
                &codecReader,
                &(const HAPTLVReaderOptions) {
                        .bytes = codecBuffer, .maxBytes = tlv.value.numBytes, .numBytes = tlv.value.numBytes });
        HAPTLV typeTLV, paramsTLV, attributesTLV;
        typeTLV.type = 1;
        paramsTLV.type = 2;
        attributesTLV.type = 3;
        err = HAPTLVReaderGetAll(&codecReader, (HAPTLV* const[]) { &typeTLV, &paramsTLV, &attributesTLV, NULL });
        HAPAssert(!err);
        HAPLogDebug(&kHAPLog_Default, "tlv type: %d\ntlv size: %lu\n", typeTLV.type, typeTLV.value.numBytes);
        HAPLogDebug(&kHAPLog_Default, "tlv type: %d\ntlv size: %lu\n", paramsTLV.type, paramsTLV.value.numBytes);
        HAPLogDebug(
                &kHAPLog_Default, "tlv type: %d\ntlv size: %lu\n", attributesTLV.type, attributesTLV.value.numBytes);
    }
    return err;
};

HAP_RESULT_USE_CHECK
HAPError HandleStreamingStatusRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    HAPLogInfo(&kHAPLog_Default, "streaming state: %d", accessoryConfiguration.state.streaming);
    HAPError err;

    err = HAPTLVWriterAppend(
            responseWriter,
            &(const HAPTLV) { .type = 1, // Streaming Status
                              .value = { .bytes = &accessoryConfiguration.state.streaming, .numBytes = 1 } });
    HAPAssert(!err);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleSupportedAudioRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter HAP_UNUSED,
        void* _Nullable context HAP_UNUSED) {

    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    return HAPTLVWriterEncode(responseWriter, &supportedAudioConfigFormat, &supportedAudioConfigValue);
}

HAP_RESULT_USE_CHECK
HAPError HandleSupportedVideoRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter HAP_UNUSED,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    HAPError err;

    err = HAPTLVWriterEncode(responseWriter, &supportedVideoConfigFormat, &supportedVideoConfigValue);
    HAPAssert(!err);
    void* actualBytes;
    size_t numActualBytes;
    HAPTLVWriterGetBuffer(responseWriter, &actualBytes, &numActualBytes);
    err = VerifyCodecConfigTLV(actualBytes, numActualBytes);

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleSupportedRTPConfigRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(responseWriter);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    uint8_t supportedSRTPcryptoSuite = 0; // 0 - AES_CM_128_HMAC_SHA1_80

    err = HAPTLVWriterAppend(
            responseWriter,
            &(const HAPTLV) { .type = 2, // SRTP Crypto Suite
                              .value = { .bytes = &supportedSRTPcryptoSuite, .numBytes = 1 } });
    HAPAssert(!err);

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleSelectedRTPConfigRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter HAP_UNUSED,
        void* _Nullable context HAP_UNUSED) {

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleSelectedRTPConfigWrite(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicWriteRequest* request HAP_UNUSED,
        HAPTLVReaderRef* requestReader,
        void* _Nullable context HAP_UNUSED) {
    HAPPrecondition(requestReader);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;

    // Simply validate input.
    err = HAPTLVReaderGetAll(requestReader, (HAPTLV* const[]) { NULL });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleSetupEndpointsRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicReadRequest* request HAP_UNUSED,
        HAPTLVWriterRef* responseWriter HAP_UNUSED,
        void* _Nullable context HAP_UNUSED) {

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);
    /*
        HAPError err;

        void* bytes;
        size_t maxBytes;
        HAPTLVWriterGetScratchBytes(responseWriter, &bytes, &maxBytes);

        err = HAPTLVWriterAppend(
                responseWriter,
                &(const HAPTLV) { .type = 2, // Status
                                  .value = { .bytes = &accessoryConfiguration.state.streaming, .numBytes = 1 } });

        err = HAPTLVWriterAppend(
                responseWriter,
                &(const HAPTLV) { .type = 3, // Address
                                  .value = { .bytes = &accessoryConfiguration.ipAddress, .numBytes = 9 } });

        HAPAssert(!err);
     */
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleSetupEndpointsWrite(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPTLV8CharacteristicWriteRequest* request HAP_UNUSED,
        HAPTLVReaderRef* requestReader,
        void* _Nullable context HAP_UNUSED) {
    // HAPPrecondition(requestReader);

    HAPLogInfo(&kHAPLog_Default, "%s", __func__);

    HAPError err;
    HAPTLV sessionId;
    bool found;

    // Simply validate input.
    err = HAPTLVReaderGetNext(requestReader, &found, &sessionId);
    // err = HAPTLVReaderGetAll(requestReader, (HAPTLV* const[]) { &sessionId });
    if (err) {
        HAPAssert(err == kHAPError_InvalidData);
        return err;
    }

    if (found) {
        HAPLogInfo(&kHAPLog_Default, "Found a TLV");
    }

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleMicMuteRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPBoolCharacteristicReadRequest* request HAP_UNUSED,
        bool* value,
        void* _Nullable context HAP_UNUSED) {
    *value = accessoryConfiguration.state.microphone.muted;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");

    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleMicMuteWrite(
        HAPAccessoryServerRef* server,
        const HAPBoolCharacteristicWriteRequest* request,
        bool value,
        void* _Nullable context HAP_UNUSED) {
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, value ? "true" : "false");
    if (accessoryConfiguration.state.microphone.muted != value) {
        accessoryConfiguration.state.microphone.muted = value;

        SaveAccessoryState();

        HAPAccessoryServerRaiseEvent(server, request->characteristic, request->service, request->accessory);
    }
    return kHAPError_None;
}

HAP_RESULT_USE_CHECK
HAPError HandleMotionDetectedRead(
        HAPAccessoryServerRef* server HAP_UNUSED,
        const HAPBoolCharacteristicReadRequest* request HAP_UNUSED,
        bool* value,
        void* _Nullable context HAP_UNUSED) {
    *value = accessoryConfiguration.state.motion.detected;
    HAPLogInfo(&kHAPLog_Default, "%s: %s", __func__, *value ? "true" : "false");
    return kHAPError_None;
}
