/** @file
  Random Number Generator (RNG) GUIDs and structures shared across RNG interfaces.

  Copyright (c) 2013 - 2018, Intel Corporation. All rights reserved.<BR>
  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef RNG_GUID_H_
#define RNG_GUID_H_

FILE_LICENCE ( BSD2_PATENT );

typedef struct _EFI_RNG_INTERFACE EFI_RNG_INTERFACE;

///
/// A selection of EFI_RNG_PROTOCOL algorithms.
/// The algorithms listed are optional, not meant to be exhaustive and be argmented by
/// vendors or other industry standards.
///
typedef EFI_GUID EFI_RNG_ALGORITHM;

///
/// The algorithms corresponds to SP800-90 as defined in
/// NIST SP 800-90, "Recommendation for Random Number Generation Using Deterministic Random
/// Bit Generators", March 2007.
///
#define EFI_RNG_ALGORITHM_SP800_90_HASH_256_GUID \
  { \
    0xa7af67cb, 0x603b, 0x4d42, {0xba, 0x21, 0x70, 0xbf, 0xb6, 0x29, 0x3f, 0x96 } \
  }
#define EFI_RNG_ALGORITHM_SP800_90_HMAC_256_GUID \
  { \
    0xc5149b43, 0xae85, 0x4f53, {0x99, 0x82, 0xb9, 0x43, 0x35, 0xd3, 0xa9, 0xe7 } \
  }
#define EFI_RNG_ALGORITHM_SP800_90_CTR_256_GUID \
  { \
    0x44f0de6e, 0x4d8c, 0x4045, {0xa8, 0xc7, 0x4d, 0xd1, 0x68, 0x85, 0x6b, 0x9e } \
  }

///
/// The algorithms correspond to X9.31 as defined in
/// NIST, "Recommended Random Number Generator Based on ANSI X9.31 Appendix A.2.4 Using
/// the 3-Key Triple DES and AES Algorithm", January 2005.
///
#define EFI_RNG_ALGORITHM_X9_31_3DES_GUID \
  { \
    0x63c4785a, 0xca34, 0x4012, {0xa3, 0xc8, 0x0b, 0x6a, 0x32, 0x4f, 0x55, 0x46 } \
  }
#define EFI_RNG_ALGORITHM_X9_31_AES_GUID \
  { \
    0xacd03321, 0x777e, 0x4d3d, {0xb1, 0xc8, 0x20, 0xcf, 0xd8, 0x88, 0x20, 0xc9 } \
  }

///
/// The "raw" algorithm, when supported, is intended to provide entropy directly from
/// the source, without it going through some deterministic random bit generator.
///
#define EFI_RNG_ALGORITHM_RAW \
  { \
    0xe43176d7, 0xb6e8, 0x4827, {0xb7, 0x84, 0x7f, 0xfd, 0xc4, 0xb6, 0x85, 0x61 } \
  }

///
/// The Arm Architecture states the RNDR that the DRBG algorithm should be compliant
/// with NIST SP800-90A, while not mandating a particular algorithm, so as to be
/// inclusive of different geographies.
///
#define EFI_RNG_ALGORITHM_ARM_RNDR \
  { \
    0x43d2fde3, 0x9d4e, 0x4d79,  {0x02, 0x96, 0xa8, 0x9b, 0xca, 0x78, 0x08, 0x41} \
  }

/**
  Returns information about the random number generation implementation.

  @param[in]     This                 A pointer to this interface instance.
  @param[in,out] RNGAlgorithmListSize On input, the size in bytes of RNGAlgorithmList.
                                      On output with a return code of EFI_SUCCESS, the size
                                      in bytes of the data returned in RNGAlgorithmList. On output
                                      with a return code of EFI_BUFFER_TOO_SMALL,
                                      the size of RNGAlgorithmList required to obtain the list.
  @param[out] RNGAlgorithmList        A caller-allocated memory buffer filled by the driver
                                      with one EFI_RNG_ALGORITHM element for each supported
                                      RNG algorithm. The list must not change across multiple
                                      calls to the same driver. The first algorithm in the list
                                      is the default algorithm for the driver.

  @retval EFI_SUCCESS                 The RNG algorithm list was returned successfully.
  @retval EFI_UNSUPPORTED             The services is not supported by this driver.
  @retval EFI_DEVICE_ERROR            The list of algorithms could not be retrieved due to a
                                      hardware or firmware error.
  @retval EFI_INVALID_PARAMETER       One or more of the parameters are incorrect.
  @retval EFI_BUFFER_TOO_SMALL        The buffer RNGAlgorithmList is too small to hold the result.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_RNG_GET_INFO)(
  IN EFI_RNG_INTERFACE            *This,
  IN OUT UINTN                    *RNGAlgorithmListSize,
  OUT EFI_RNG_ALGORITHM           *RNGAlgorithmList
  );

/**
  Produces and returns an RNG value using either the default or specified RNG algorithm.

  @param[in]  This                    A pointer to this interface instance.
  @param[in]  RNGAlgorithm            A pointer to the EFI_RNG_ALGORITHM that identifies the RNG
                                      algorithm to use. May be NULL in which case the function will
                                      use its default RNG algorithm.
  @param[in]  RNGValueLength          The length in bytes of the memory buffer pointed to by
                                      RNGValue. The driver shall return exactly this numbers of bytes.
  @param[out] RNGValue                A caller-allocated memory buffer filled by the driver with the
                                      resulting RNG value.

  @retval EFI_SUCCESS                 The RNG value was returned successfully.
  @retval EFI_UNSUPPORTED             The algorithm specified by RNGAlgorithm is not supported by
                                      this driver.
  @retval EFI_DEVICE_ERROR            An RNG value could not be retrieved due to a hardware or
                                      firmware error.
  @retval EFI_NOT_READY               There is not enough random data available to satisfy the length
                                      requested by RNGValueLength.
  @retval EFI_INVALID_PARAMETER       RNGValue is NULL or RNGValueLength is zero.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_RNG_GET_RNG)(
  IN EFI_RNG_INTERFACE           *This,
  IN EFI_RNG_ALGORITHM           *RNGAlgorithm  OPTIONAL,
  IN UINTN                       RNGValueLength,
  OUT UINT8                      *RNGValue
  );

///
/// The Random Number Generator (RNG) interface provides random bits for use in
/// applications, or entropy for seeding other random number generators.
///
/// This interface is shared between the RNG Protocol defined in the UEFI 2.4 Specification
/// and the RNG PPI defined in the PI 1.9 Specification.
///
struct _EFI_RNG_INTERFACE {
  EFI_RNG_GET_INFO    GetInfo;
  EFI_RNG_GET_RNG     GetRNG;
};

extern EFI_GUID  gEfiRngAlgorithmSp80090Hash256Guid;
extern EFI_GUID  gEfiRngAlgorithmSp80090Hmac256Guid;
extern EFI_GUID  gEfiRngAlgorithmSp80090Ctr256Guid;
extern EFI_GUID  gEfiRngAlgorithmX9313DesGuid;
extern EFI_GUID  gEfiRngAlgorithmX931AesGuid;
extern EFI_GUID  gEfiRngAlgorithmRaw;
extern EFI_GUID  gEfiRngAlgorithmArmRndr;

#endif // #ifndef RNG_GUID_H_
