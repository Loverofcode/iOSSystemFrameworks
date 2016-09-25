//
//  UIFont.h
//  UIKit
//
//  Copyright (c) 2007-2015 Apple Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIKitDefines.h>
#import <UIKit/UIFontDescriptor.h>

NS_ASSUME_NONNULL_BEGIN

// 字体
NS_CLASS_AVAILABLE_IOS(2_0) @interface UIFont : NSObject <NSCopying>

// Returns an instance of the font associated with the text style and scaled appropriately for the user's selected content size category. See UIFontDescriptor.h for the complete list.
+ (UIFont *)preferredFontForTextStyle:(NSString *)style NS_AVAILABLE_IOS(7_0);

// Returns a font using CSS name matching semantics.
+ (nullable UIFont *)fontWithName:(NSString *)fontName size:(CGFloat)fontSize;

// Returns an array of font family names for all installed fonts
// 返回所有可用字体名
+ (NSArray<NSString *> *)familyNames;

// Returns an array of font names for the specified family name
// 返回某一类字体可用的字体名
+ (NSArray<NSString *> *)fontNamesForFamilyName:(NSString *)familyName;


// Some convenience methods to create system fonts

// Think carefully before using these methods. In most cases, a font returned by +preferredFontForTextStyle: will be more appropriate, and will respect the user's selected content size category.
// 指定尺寸的系统字体;粗体;斜体
+ (UIFont *)systemFontOfSize:(CGFloat)fontSize;
+ (UIFont *)boldSystemFontOfSize:(CGFloat)fontSize;
+ (UIFont *)italicSystemFontOfSize:(CGFloat)fontSize;

// Weights used here are analogous to those used with UIFontDescriptor's UIFontWeightTrait.
// See the UIFontWeight... constants in UIFontDescriptor.h for suggested values.
// The caveat above about the use of ...systemFont... methods applies to these methods too.
+ (UIFont *)systemFontOfSize:(CGFloat)fontSize weight:(CGFloat)weight NS_AVAILABLE_IOS(8_2);
+ (UIFont *)monospacedDigitSystemFontOfSize:(CGFloat)fontSize weight:(CGFloat)weight NS_AVAILABLE_IOS(9_0);


// Font attributes
@property(nonatomic,readonly,strong) NSString *familyName;
@property(nonatomic,readonly,strong) NSString *fontName;
@property(nonatomic,readonly)        CGFloat   pointSize;
@property(nonatomic,readonly)        CGFloat   ascender;
@property(nonatomic,readonly)        CGFloat   descender;
@property(nonatomic,readonly)        CGFloat   capHeight;
@property(nonatomic,readonly)        CGFloat   xHeight;
@property(nonatomic,readonly)        CGFloat   lineHeight NS_AVAILABLE_IOS(4_0);
@property(nonatomic,readonly)        CGFloat   leading;

// Create a new font that is identical to the current font except the specified size
- (UIFont *)fontWithSize:(CGFloat)fontSize;

// Returns a font matching the font descriptor. If fontSize is greater than 0.0, it has precedence over UIFontDescriptorSizeAttribute in fontDescriptor.
+ (UIFont *)fontWithDescriptor:(UIFontDescriptor *)descriptor size:(CGFloat)pointSize NS_AVAILABLE_IOS(7_0);

// Returns a font descriptor which describes the font.
- (UIFontDescriptor *)fontDescriptor NS_AVAILABLE_IOS(7_0);

@end

NS_ASSUME_NONNULL_END

 
