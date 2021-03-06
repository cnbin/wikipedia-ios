/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#import <FBSnapshotTestCase/FBSnapshotTestCasePlatform.h>

#import <QuartzCore/QuartzCore.h>

#import <UIKit/UIKit.h>

#import <XCTest/XCTest.h>

/**
 Similar to our much-loved XCTAssert() macros. Use this to perform your test. No need to write an explanation, though.
 @param view The view to snapshot
 @param identifier An optional identifier, used if there are multiple snapshot tests in a given -test method.
 @param suffixes An NSOrderedSet of strings for the different suffixes
 @param tolerance The percentage of pixels that can differ and still count as an 'identical' view
 */
#define FBSnapshotVerifyViewWithOptions(view__, identifier__, suffixes__, tolerance__) \
{ \
NSString *envReferenceImageDirectory = [NSProcessInfo processInfo].environment[@"FB_REFERENCE_IMAGE_DIR"]; \
NSError *error__ = nil; \
BOOL comparisonSuccess__; \
XCTAssertTrue((suffixes__.count > 0), @"Suffixes set cannot be empty %@", suffixes__); \
XCTAssertNotNil(envReferenceImageDirectory, @"Missing value for referenceImagesDirectory - Set FB_REFERENCE_IMAGE_DIR as Environment variable in your scheme.");\
for (NSString *suffix__ in suffixes__) { \
NSString *referenceImagesDirectory__ = [NSString stringWithFormat:@"%@%@", envReferenceImageDirectory, suffix__]; \
comparisonSuccess__ = [self compareSnapshotOfView:(view__) referenceImagesDirectory:referenceImagesDirectory__ identifier:(identifier__) tolerance:(tolerance__) error:&error__]; \
if (comparisonSuccess__ || self.recordMode) break; \
} \
XCTAssertTrue(comparisonSuccess__, @"Snapshot comparison failed: %@", error__); \
XCTAssertFalse(self.recordMode, @"Test ran in record mode. Reference image is now saved. Disable record mode to perform an actual snapshot comparison!"); \
}

#define FBSnapshotVerifyView(view__, identifier__) \
{ \
FBSnapshotVerifyViewWithOptions(view__, identifier__, FBSnapshotTestCaseDefaultSuffixes(), 0); \
}

/**
 Similar to our much-loved XCTAssert() macros. Use this to perform your test. No need to write an explanation, though.
 @param layer The layer to snapshot
 @param identifier An optional identifier, used is there are multiple snapshot tests in a given -test method.
 @param suffixes An NSOrderedSet of strings for the different suffixes
 @param tolerance The percentage of pixels that can differ and still count as an 'identical' layer
 */
#define FBSnapshotVerifyLayerWithOptions(layer__, identifier__, suffixes__, tolerance__) \
{ \
NSString *envReferenceImageDirectory = [NSProcessInfo processInfo].environment[@"FB_REFERENCE_IMAGE_DIR"]; \
NSError *error__ = nil; \
BOOL comparisonSuccess__; \
XCTAssertTrue((suffixes__.count > 0), @"Suffixes set cannot be empty %@", suffixes__); \
XCTAssertNotNil(envReferenceImageDirectory, @"Missing value for referenceImagesDirectory - Set FB_REFERENCE_IMAGE_DIR as Environment variable in your scheme.");\
for (NSString *suffix__ in suffixes__) { \
NSString *referenceImagesDirectory__ = [NSString stringWithFormat:@"%@%@", envReferenceImageDirectory, suffix__]; \
comparisonSuccess__ = [self compareSnapshotOfLayer:(layer__) referenceImagesDirectory:referenceImagesDirectory__ identifier:(identifier__) tolerance:(tolerance__) error:&error__]; \
if (comparisonSuccess__ || self.recordMode) break; \
} \
XCTAssertTrue(comparisonSuccess__, @"Snapshot comparison failed: %@", error__); \
XCTAssertFalse(self.recordMode, @"Test ran in record mode. Reference image is now saved. Disable record mode to perform an actual snapshot comparison!"); \
}

#define FBSnapshotVerifyLayer(layer__, identifier__) \
{ \
FBSnapshotVerifyLayerWithOptions(layer__, identifier__, FBSnapshotTestCaseDefaultSuffixes(), 0); \
}

/**
 The base class of view snapshotting tests. If you have small UI component, it's often easier to configure it in a test
 and compare an image of the view to a reference image that write lots of complex layout-code tests.

 In order to flip the tests in your subclass to record the reference images set @c recordMode to @c YES.

 For example:
 @code
 - (void)setUp
 {
    [super setUp];
    self.recordMode = YES;
 }
 @endcode
 */
@interface FBSnapshotTestCase : XCTestCase

/**
 When YES, the test macros will save reference images, rather than performing an actual test.
 */
@property (readwrite, nonatomic, assign) BOOL recordMode;

/**
 When YES, renders a snapshot of the complete view hierarchy as visible onscreen.
 There are several things that do not work if renderInContext: is used.
 - UIVisualEffect #70
 - UIAppearance #91
 - Size Classes #92

 @attention If the view does't belong to a UIWindow, it will create one and add the view as a subview.
 */
@property (readwrite, nonatomic, assign) BOOL usesDrawViewHierarchyInRect;

- (void)setUp NS_REQUIRES_SUPER;
- (void)tearDown NS_REQUIRES_SUPER;

/**
 Performs the comparison or records a snapshot of the layer if recordMode is YES.
 @param layer The Layer to snapshot
 @param referenceImagesDirectory The directory in which reference images are stored.
 @param identifier An optional identifier, used if there are multiple snapshot tests in a given -test method.
 @param tolerance The percentage difference to still count as identical - 0 mean pixel perfect, 1 means I don't care
 @param errorPtr An error to log in an XCTAssert() macro if the method fails (missing reference image, images differ, etc).
 @returns YES if the comparison (or saving of the reference image) succeeded.
 */
- (BOOL)compareSnapshotOfLayer:(CALayer *)layer
      referenceImagesDirectory:(NSString *)referenceImagesDirectory
                    identifier:(NSString *)identifier
                     tolerance:(CGFloat)tolerance
                         error:(NSError **)errorPtr;

/**
 Performs the comparison or records a snapshot of the view if recordMode is YES.
 @param view The view to snapshot
 @param referenceImagesDirectory The directory in which reference images are stored.
 @param identifier An optional identifier, used if there are multiple snapshot tests in a given -test method.
 @param tolerance The percentage difference to still count as identical - 0 mean pixel perfect, 1 means I don't care
 @param errorPtr An error to log in an XCTAssert() macro if the method fails (missing reference image, images differ, etc).
 @returns YES if the comparison (or saving of the reference image) succeeded.
 */
- (BOOL)compareSnapshotOfView:(UIView *)view
     referenceImagesDirectory:(NSString *)referenceImagesDirectory
                   identifier:(NSString *)identifier
                    tolerance:(CGFloat)tolerance
                        error:(NSError **)errorPtr;

@end
