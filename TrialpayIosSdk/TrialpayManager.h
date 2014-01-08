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
    @mainpage

    Trialpay iOS SDK API Reference Documentation.

    Thanks for using Trialpay!

    Follow the @ref SampleIntegration or browse through the @ref TrialpayManager for details on each API feature.

     @see <a href="http://help.trialpay.com/mobile/ios-sdk">Integrating the TrialPay SDK into an iOS app</a>
     @see @ref TrialpayManager
     @see @ref TrialpayManagerDelegate
     @see @ref SampleIntegration

     @page SampleIntegration Sample Integration

     @code

... On your view controller

#import "TrialpayManager.h"

...

@implementation MyViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    TrialpayManager* trialpayManager = [TrialpayManager getInstance];
    [trialpayManager setSid:@"SomeDeviceUserId"];
    [trialpayManager registerVic:@"<INTEGRATION_CODE_FROM_TRIALPAY_PRODUCTS_PAGE>" withTouchpoint:@""];
//    [trialpayManager initiateBalanceChecks]; // uncomment if using Balance API (See section 3.6.2), should be called only once...
    [trialpayManager setDelegate:self]; // get responses from Trialpay
}

// Associate this IBAction with the button that triggers the offerwall
- (IBAction)openOfferwallWithSender:(id)sender {
    TrialpayManager *trialpayManager = [TrialpayManager getInstance];
    [trialpayManager openOfferwallForTouchpoint:@"openOfferwall"];
}

// Listen to Trialpay Actions
- (void)trialpayManager:(TrialpayManager *)trialpayManager withAction:(TPMessageAction)action {
  NSLog(@"Got a Trialpay event");
  switch (action) {
    case TPOfferwallCloseAction:
      NSLog(@"Trialpay Offerwall is now closed");
      break;
    case TPBalanceUpdateAction: // uncomment if using Balance API (See section 3.6.2)
//      int balanceToAdd = [trialpayManager withdrawBalanceForTouchpoint:MY_TOUCHPOINT];
//      NSLog(@"User was credited %d coins!", balanceToAdd);
      break;
  }
}

...

@end

@endcode

 */

/*!
     This is the class used to perform all SDK tasks.

     Terminology:
     - <b>Device user</b>: the user that is using the device.
     - <b>VIC</b>: the campaign identification, also called "Integration Code". Find it on your Merchant page/Products, the item "Get integration code" under every campaign.
     - <b>SID</b>: an unique device user identification. Its preferably provided by you, but can be generated by Trialpay.
     - <b>Touchpoint</b>: identifies the button that was clicked to open the Offerwall - the name is your choice.


     @see <a href="http://help.trialpay.com/mobile/ios-sdk">Integrating the TrialPay SDK into an iOS app</a>
     @see @ref SampleIntegration
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
+ (NSString*)sdkVersion;

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
    Open the Trialpay Offerwall for a given touchpoint.
    @param touchpointName The touchpoint.
*/
- (void)openOfferwallForTouchpoint:(NSString *)touchpointName;

/*!
    Create the Trialpay Dealspot for a given touchpoint.
    @param touchpointName The touchpoint.
    @param touchpointFrame The frame for the touchpoint icon
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
    Set the age of the device user. Affects all touchpoints.
    This method should be called on device user registration or during initialization.
    @param age The age of device user.
*/
- (void)setAge:(int)age;
/*!
    Set the gender of the device user. Affects all touchpoints.
    This method should be called on device user registration or during initialization.
    @param gender The gender of the device user.
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
    Delegate for Offerwall close events and Balance update events, See TrialpayManagerDelegate.
    @include updateVcBalanceForTouchpoint.m
*/
@property (strong, nonatomic) id<TrialpayManagerDelegate> delegate;


@end
