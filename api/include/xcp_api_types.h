//
// Created by sauci on 16/11/2018.
//

#ifndef XCPUI_XCP_API_TYPES_H
#define XCPUI_XCP_API_TYPES_H

#include <cstdint>

namespace xcp_api_types
{
    typedef std::uint32_t identifier_type;

    typedef std::uint32_t baud_rate_type;

    typedef enum : std::uint16_t
    {
        T1 = 1,
        T2 = 2,
        T3 = 3,
        T4 = 4,
        T5 = 5,
        T6 = 6,
        T7 = 7
    } timing_parameter_id_type;

    typedef std::uint16_t timing_parameter_type;

    typedef std::uint32_t status_type;

    typedef const std::uint32_t placeholder_type;

    typedef enum
    {
        CTO,
        DTO
    } message_type;
}

namespace xcp_types
{
    namespace connect
    {
        typedef enum : std::uint8_t
        {
            NORMAL = 0,
            USER_DEFINED = 1
        } MODE;
    }

    namespace get_id
    {
        typedef enum : std::uint8_t
        {
            ASCII_TEXT = 0,
            ASAM_MC2_NO_PATH = 1,
            ASAM_MC2_PATH = 2,
            ASAM_MC2_URL = 3,
            ASAM_MC2_UPLOAD = 4,
            USER_DEFINED_128 = 128,
            USER_DEFINED_129 = 129,
            USER_DEFINED_130 = 130,
            USER_DEFINED_131 = 131,
            USER_DEFINED_132 = 132,
            USER_DEFINED_133 = 133,
            USER_DEFINED_134 = 134,
            USER_DEFINED_135 = 135,
            USER_DEFINED_136 = 136,
            USER_DEFINED_137 = 137,
            USER_DEFINED_138 = 138,
            USER_DEFINED_139 = 139,
            USER_DEFINED_140 = 140,
            USER_DEFINED_141 = 141,
            USER_DEFINED_142 = 142,
            USER_DEFINED_143 = 143,
            USER_DEFINED_144 = 144,
            USER_DEFINED_145 = 145,
            USER_DEFINED_146 = 146,
            USER_DEFINED_147 = 147,
            USER_DEFINED_148 = 148,
            USER_DEFINED_149 = 149,
            USER_DEFINED_150 = 150,
            USER_DEFINED_151 = 151,
            USER_DEFINED_152 = 152,
            USER_DEFINED_153 = 153,
            USER_DEFINED_154 = 154,
            USER_DEFINED_155 = 155,
            USER_DEFINED_156 = 156,
            USER_DEFINED_157 = 157,
            USER_DEFINED_158 = 158,
            USER_DEFINED_159 = 159,
            USER_DEFINED_160 = 160,
            USER_DEFINED_161 = 161,
            USER_DEFINED_162 = 162,
            USER_DEFINED_163 = 163,
            USER_DEFINED_164 = 164,
            USER_DEFINED_165 = 165,
            USER_DEFINED_166 = 166,
            USER_DEFINED_167 = 167,
            USER_DEFINED_168 = 168,
            USER_DEFINED_169 = 169,
            USER_DEFINED_170 = 170,
            USER_DEFINED_171 = 171,
            USER_DEFINED_172 = 172,
            USER_DEFINED_173 = 173,
            USER_DEFINED_174 = 174,
            USER_DEFINED_175 = 175,
            USER_DEFINED_176 = 176,
            USER_DEFINED_177 = 177,
            USER_DEFINED_178 = 178,
            USER_DEFINED_179 = 179,
            USER_DEFINED_180 = 180,
            USER_DEFINED_181 = 181,
            USER_DEFINED_182 = 182,
            USER_DEFINED_183 = 183,
            USER_DEFINED_184 = 184,
            USER_DEFINED_185 = 185,
            USER_DEFINED_186 = 186,
            USER_DEFINED_187 = 187,
            USER_DEFINED_188 = 188,
            USER_DEFINED_189 = 189,
            USER_DEFINED_190 = 190,
            USER_DEFINED_191 = 191,
            USER_DEFINED_192 = 192,
            USER_DEFINED_193 = 193,
            USER_DEFINED_194 = 194,
            USER_DEFINED_195 = 195,
            USER_DEFINED_196 = 196,
            USER_DEFINED_197 = 197,
            USER_DEFINED_198 = 198,
            USER_DEFINED_199 = 199,
            USER_DEFINED_200 = 200,
            USER_DEFINED_201 = 201,
            USER_DEFINED_202 = 202,
            USER_DEFINED_203 = 203,
            USER_DEFINED_204 = 204,
            USER_DEFINED_205 = 205,
            USER_DEFINED_206 = 206,
            USER_DEFINED_207 = 207,
            USER_DEFINED_208 = 208,
            USER_DEFINED_209 = 209,
            USER_DEFINED_210 = 210,
            USER_DEFINED_211 = 211,
            USER_DEFINED_212 = 212,
            USER_DEFINED_213 = 213,
            USER_DEFINED_214 = 214,
            USER_DEFINED_215 = 215,
            USER_DEFINED_216 = 216,
            USER_DEFINED_217 = 217,
            USER_DEFINED_218 = 218,
            USER_DEFINED_219 = 219,
            USER_DEFINED_220 = 220,
            USER_DEFINED_221 = 221,
            USER_DEFINED_222 = 222,
            USER_DEFINED_223 = 223,
            USER_DEFINED_224 = 224,
            USER_DEFINED_225 = 225,
            USER_DEFINED_226 = 226,
            USER_DEFINED_227 = 227,
            USER_DEFINED_228 = 228,
            USER_DEFINED_229 = 229,
            USER_DEFINED_230 = 230,
            USER_DEFINED_231 = 231,
            USER_DEFINED_232 = 232,
            USER_DEFINED_233 = 233,
            USER_DEFINED_234 = 234,
            USER_DEFINED_235 = 235,
            USER_DEFINED_236 = 236,
            USER_DEFINED_237 = 237,
            USER_DEFINED_238 = 238,
            USER_DEFINED_239 = 239,
            USER_DEFINED_240 = 240,
            USER_DEFINED_241 = 241,
            USER_DEFINED_242 = 242,
            USER_DEFINED_243 = 243,
            USER_DEFINED_244 = 244,
            USER_DEFINED_245 = 245,
            USER_DEFINED_246 = 246,
            USER_DEFINED_247 = 247,
            USER_DEFINED_248 = 248,
            USER_DEFINED_249 = 249,
            USER_DEFINED_250 = 250,
            USER_DEFINED_251 = 251,
            USER_DEFINED_252 = 252,
            USER_DEFINED_253 = 253,
            USER_DEFINED_254 = 254,
            USER_DEFINED_255 = 255
        } TYPE;
    }

    namespace set_request
    {
        typedef std::uint8_t MODE;
        const MODE STORE_CAL_REQ = 0;
        const MODE STORE_DAQ_REQ = 2;
        const MODE CLEAR_DAQ_REQ = 3;

        typedef std::uint16_t SESSION_CONFIGURATION_ID;
    }

    namespace get_seed
    {
        typedef enum : std::uint8_t
        {
            FIRST_PART = 0,
            REMAINING_PART = 1
        } MODE;

        typedef enum : std::uint8_t
        {
            TRUE_ = 0,
            FALSE_ = 1
        } RESOURCE;
    }

    namespace unlock
    {
        typedef std::uint8_t KEY_LENGTH;
    }

    namespace set_mta
    {
        typedef std::uint8_t ADDRESS_EXT;
        typedef std::uint32_t ADDRESS;
    }

    namespace upload
    {
        typedef std::uint8_t NUM_OF_DATA;
    }

    namespace short_upload
    {
        typedef std::uint8_t NUM_OF_DATA;
        typedef std::uint8_t ADDRESS_EXT;
        typedef std::uint32_t ADDRESS;
    }

    namespace build_checksum
    {
        typedef std::uint32_t BLOCK_SIZE;
    }

    namespace transport_layer_cmd
    {
        typedef std::uint8_t SUB_COMMAND_CODE;
        typedef std::uint8_t PARAMETERS;
    }

    namespace usr_cmd
    {
        typedef std::uint8_t SUB_COMMAND_CODE;
        typedef std::uint8_t PARAMETERS;
    }

    namespace download
    {
        typedef std::uint8_t NUM_OF_DATA;
        typedef std::uint8_t ALIGNMENT;
        typedef union
        {
            std::uint8_t BYTE_SIZE_1[1];
            std::uint8_t BYTE_SIZE_2[2];
            std::uint8_t BYTE_SIZE_4[4];
        } DATA;
    }

    namespace download_next
    {
        typedef std::uint8_t NUM_OF_DATA;
        typedef std::uint8_t ALIGNMENT;
        typedef union
        {
            std::uint8_t BYTE_SIZE_1[1];
            std::uint8_t BYTE_SIZE_2[2];
            std::uint8_t BYTE_SIZE_4[4];
        } DATA;
    }

    namespace download_max
    {
        typedef std::uint8_t ALIGNMENT;
        typedef union
        {
            std::uint8_t BYTE_SIZE_1[1];
            std::uint32_t BYTE_SIZE_3[3];
        } DATA;
    }

    namespace short_download
    {
        typedef std::uint8_t NUM_OF_DATA;
        typedef std::uint8_t ADDRESS_EXT;
        typedef std::uint32_t ADDRESS;
        typedef union
        {
            std::uint8_t BYTE_SIZE_1[1];
            std::uint8_t BYTE_SIZE_2[2];
            std::uint8_t BYTE_SIZE_4[4];
        } DATA; /* TODO: might use template? */
    }

    typedef enum : std::uint8_t
    {
        CAL_PAG = 0,
        DAQ = (1 << 2),
        STIM = (1 << 3),
        PGM = (1 << 4)
    } RESOURCE;

    typedef std::uint8_t COMM_MODE_BASIC;
    const COMM_MODE_BASIC BYTE_ORDER = 0;
    const COMM_MODE_BASIC ADDRESS_GRANULARITY_0 = (1 << 1);
    const COMM_MODE_BASIC ADDRESS_GRANULARITY_1 = (1 << 2);
    const COMM_MODE_BASIC SLAVE_BLOCK_MODE = (1 << 6);
    const COMM_MODE_BASIC OPTIONAL_ = (1 << 7); /* TODO: undefine and redefine. */
}

#endif //XCPUI_XCP_API_TYPES_H
