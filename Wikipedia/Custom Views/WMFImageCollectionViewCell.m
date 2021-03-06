//
//  WMFImageCollectionViewCell.m
//  Wikipedia
//
//  Created by Brian Gerstle on 7/17/15.
//  Copyright (c) 2015 Wikimedia Foundation. All rights reserved.
//

#import "WMFImageCollectionViewCell.h"
#import "UIImageView+WMFContentOffset.h"
#import "UIImageView+MWKImage.h"

@implementation WMFImageCollectionViewCell

- (void)prepareForReuse {
    [super prepareForReuse];
    [self.imageView wmf_resetContentOffset];
    [self.imageView wmf_resetImageMetadata];
    self.imageView.image       = [UIImage imageNamed:@"lead-default"];
    self.imageView.contentMode = UIViewContentModeScaleAspectFill;
}

@end
