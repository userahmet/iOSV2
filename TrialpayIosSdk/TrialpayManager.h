//
//  TrialpayManager.h
//
//  Created by Trialpay Inc.
//  Copyright (c) 2013 TrialPay, Inc. All Rights Reserved.
//

#import <Foundation/Foundation.h>
#import "BaseTrialpayManager.h"
#import "TpConstants.h"

@class TpDealspotView;

/*!
     This is the class used to perform all SDK tasks.
*/
@interface TrialpayManager : BaseTrialpayManager

/*!
     Get the Trialpay Manager instance.
     @returns The Trialpay Manager instance.
 */
+ (TrialpayManager*)getInstance;
/*!
     Get the version of the SDK.
     @return The SDK Version.
 */
- (NSString*)sdkVersion;

/*!
     Set SID (device user identification). The SID is an unique user id for each device user. It will be used to uniquely identify your user with Trialpay system for monetization and customer support purposes.
     If you do not maintain a unique user id, we define one for the user by hashing different device identifiers such as IDFA and the device’s MAC address. Please note that choosing this path will prevent us from sending you server side notifications about your users activities.
     Therefore, for completion notification, please make sure to initiate the Balance Check.
    @param sid The device user identifier.
*/
- (void)setSid:(NSString *)sid;

/*!
     Retrieve the SID (device user identification).
    @return The device user identifier.
*/
- (NSString *)sid;

/*!
    Register your VIC (campaign identification) - the campaign integration code, a 32 Hex string that is being used in order to uniquely define your touchpoint.
    This is required to make this touchpoint available.
    @param vic The campaign identifier.
    @param touchpointName The touchpoint to register.
*/
- (void)registerVic:(NSString *)vic withTouchpoint:(NSString *)touchpointName;

/*!
    Open the Trialpay Offer Wall for a given touchpoint.
    @param touchpointName The touchpoint.

    @deprecated - Please use openTouchpoint:
*/
- (void)openOfferwallForTouchpoint:(NSString *)touchpointName __attribute__((deprecated));
/*!
    Open the touchpoint. This function should be used for Offerwall and Interstitial touchpoints.
    @param touchpointName The touchpoint
*/
- (void)openTouchpoint:(NSString *)touchpointName;

/*!
    Open the touchpoint. This function can be used for Offerwall and Interstitial touchpoints. To use the PTM flow, call this function in popup mode.
    @param touchpointName The touchpoint
    @param mode Choose between fullscreen (like regular offerwall) and popup mode (for PTM flows)
*/
- (void)openTouchpoint:(NSString *)touchpointName withMode:(TPViewMode)mode;

/*!
    Create the Trialpay Dealspot for a given touchpoint.
    @param touchpointName The touchpoint.
    @param touchpointFrame The frame for the touchpoint icon
    @return A view element that should be added as the touchpoint to the UI
*/
- (TpDealspotView *)createDealspotViewForTouchpoint:(NSString *)touchpointName withFrame:(CGRect)touchpointFrame;

/*!
    Stop and remove from UI the Trialpay Dealspot for a given touchpoint.
    @param touchpointName The touchpoint.
*/
- (void)stopDealspotViewForTouchpoint:(NSString *)touchpointName;

/*!
    Start balance checks. Call this method only once, within your application:didFinishLaunchingWithOptions:.
    @note Only start balance checks if you enabled the Balance Check API
*/
- (void)initiateBalanceChecks;
/*!
    Withdraw a differential balance for a given touchpoint.
    @note The differential balance is only what was earned, it does NOT refer to the total balance the user ever received.
    @param touchpointName The touchpoint to withdraw from.
    @return The differential balance the device user received.
*/
- (int)withdrawBalanceForTouchpoint:(NSString *)touchpointName;

/*!
    Start availability checks. Call this method only once, within your application:didFinishLaunchingWithOptions:.
    @param touchpointName The touchpoint name, as regisetered in the registerVic:withTouchpoint: method
    @note Only start availability checks for Interstitial flow.
*/
- (void)startAvailabilityCheckForTouchpoint:(NSString *)touchpointName;

/*!
    Check whether a touchpoint is available
    @param touchpointName The touchpoint name, as regisetered in the registerVic:withTouchpoint: method
    @return true if the touchpoint was correctly registered and there's content available for it
    @note Check availability only for Interstitial flows
 */
- (BOOL)isAvailableTouchpoint:(NSString *)touchpointName;

/*!
    Set the age of the device user. Affects all touchpoints.
    This method should be called on device user registration or during initialization.
    @param age The age of device user.
*/
- (void)setAge:(int)age;
/*!
    Set the gender of the device user. Affects all touchpoints.
    This method should be called on device user registration or during initialization.
    @param gender The gender of the device user. Values can be Female, Male or Unknown
 */
- (void)setGender:(Gender)gender;
/*!
    Set the level of the user on this game. Affects all touchpoints.
    This method should be called whenever there's a level/stage update in the game (showing the maximum enabled level for the user). Can be called right before the touchpoint is being loaded.
    @param level The new level of the device user.
 */
- (void)updateLevel:(int)level;

/*!
    This method stores custom parameters. All set parameters (even if they have a value of an empty string) will be passed on API calls.
    If the paramValue is set to Null, the passed value will be "" (empty string).
    @param paramValue The value of the parameter.
    @param paramName The name of the parameter.
*/
- (void)setCustomParamValue:(NSString *)paramValue forName:(NSString *)paramName;

/*!
    This method clears custom parameters.
    @param paramName The name of the parameter to clear.
 */
- (void)clearCustomParamWithName:(NSString *)paramName;
/*!
    Stores, for each touchpoint and aggregate the dollarAmount and vcAmount.
    This method should be called by the developer when an IAP purchase is done.
    It allows to track the life time dollar amount spent by that device user and the life time virtual currency amount purchased.
    It should also be used if the device user can gain virtual currency in the game without purchasing it (dollarAmount can be 0)

    @param touchpointName The touchpoint that is getting credited.
    @param vcAmount The amount of virtual currency to add to the balance.
    @param dollarAmount The amount of currency (dollars) to add to the balance.
 */
- (void)updateVcPurchaseInfoForTouchpoint:(NSString *)touchpointName dollarAmount:(float)dollarAmount vcAmount:(int)vcAmount;
/*!
    Should be called whenever there's a virtual currency activity (or right before the touchpoint is being loaded)
    @param vcAmount The amount of virtual currency to add to the balance.
    @param touchpointName The touchpoint that is getting credited.
 */
- (void)updateVcBalanceForTouchpoint:(NSString *)touchpointName vcAmount:(int)vcAmount;

/*!
    Delegate for Offer Wall close events and Balance update events, See TrialpayManagerDelegate.
    @include updateVcBalanceForTouchpoint.m
*/
@property (strong, nonatomic) id<TrialpayManagerDelegate> delegate;


@end
