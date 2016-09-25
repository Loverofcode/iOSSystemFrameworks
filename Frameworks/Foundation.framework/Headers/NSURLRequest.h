/*	
    NSURLRequest.h
    Copyright (c) 2003-2015, Apple Inc. All rights reserved.    
    
    Public header file.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>

@class NSData;
@class NSDictionary<KeyType, ObjectType>;
@class NSInputStream;
@class NSString;
@class NSURL;
@class NSURLRequestInternal;

NS_ASSUME_NONNULL_BEGIN

/*!
    @header NSURLRequest.h

    This header file describes the constructs used to represent URL
    load requests in a manner independent of protocol and URL scheme.
    Immutable and mutable variants of this URL load request concept
    are described, named NSURLRequest and NSMutableURLRequest,
    respectively. A collection of constants is also declared to
    exercise control over URL content caching policy.

    <p>NSURLRequest and NSMutableURLRequest are designed to be
    customized to support protocol-specific requests. Protocol
    implementors who need to extend the capabilities of NSURLRequest
    and NSMutableURLRequest are encouraged to provide categories on
    these classes as appropriate to support protocol-specific data. To
    store and retrieve data, category methods can use the
    <tt>+propertyForKey:inRequest:</tt> and
    <tt>+setProperty:forKey:inRequest:</tt> class methods on
    NSURLProtocol. See the NSHTTPURLRequest on NSURLRequest and
    NSMutableHTTPURLRequest on NSMutableURLRequest for examples of
    such extensions.

    <p>The main advantage of this design is that a client of the URL
    loading library can implement request policies in a standard way
    without type checking of requests or protocol checks on URLs. Any
    protocol-specific details that have been set on a URL request will
    be used if they apply to the particular URL being loaded, and will
    be ignored if they do not apply.
*/

/*!
    @enum NSURLRequestCachePolicy

    @discussion The NSURLRequestCachePolicy enum defines constants that
    can be used to specify the type of interactions that take place with
    the caching system when the URL loading system processes a request.
    Specifically, these constants cover interactions that have to do
    with whether already-existing cache data is returned to satisfy a
    URL load request.

    @constant NSURLRequestUseProtocolCachePolicy Specifies that the
    caching logic defined in the protocol implementation, if any, is
    used for a particular URL load request. This is the default policy
    for URL load requests.

    @constant NSURLRequestReloadIgnoringLocalCacheData Specifies that the
    data for the URL load should be loaded from the origin source. No
    existing local cache data, regardless of its freshness or validity,
    should be used to satisfy a URL load request.

    @constant NSURLRequestReloadIgnoringLocalAndRemoteCacheData Specifies that
    not only should the local cache data be ignored, but that proxies and
    other intermediates should be instructed to disregard their caches
    so far as the protocol allows.  Unimplemented.

    @constant NSURLRequestReloadIgnoringCacheData Older name for
    NSURLRequestReloadIgnoringLocalCacheData.

    @constant NSURLRequestReturnCacheDataElseLoad Specifies that the
    existing cache data should be used to satisfy a URL load request,
    regardless of its age or expiration date. However, if there is no
    existing data in the cache corresponding to a URL load request,
    the URL is loaded from the origin source.

    @constant NSURLRequestReturnCacheDataDontLoad Specifies that the
    existing cache data should be used to satisfy a URL load request,
    regardless of its age or expiration date. However, if there is no
    existing data in the cache corresponding to a URL load request, no
    attempt is made to load the URL from the origin source, and the
    load is considered to have failed. This constant specifies a
    behavior that is similar to an "offline" mode.

    @constant NSURLRequestReloadRevalidatingCacheData Specifies that
    the existing cache data may be used provided the origin source
    confirms its validity, otherwise the URL is loaded from the
    origin source.  Unimplemented.
*/
// 请求的缓存处理策略    
typedef NS_ENUM(NSUInteger, NSURLRequestCachePolicy)
{
    NSURLRequestUseProtocolCachePolicy = 0,

    NSURLRequestReloadIgnoringLocalCacheData = 1,
    NSURLRequestReloadIgnoringLocalAndRemoteCacheData = 4, // Unimplemented
    NSURLRequestReloadIgnoringCacheData = NSURLRequestReloadIgnoringLocalCacheData,

    NSURLRequestReturnCacheDataElseLoad = 2,
    NSURLRequestReturnCacheDataDontLoad = 3,

    NSURLRequestReloadRevalidatingCacheData = 5, // Unimplemented
};

/*!
 @enum NSURLRequestNetworkServiceType
 
 @discussion The NSURLRequestNetworkServiceType enum defines constants that
 can be used to specify the service type to associate with this request.  The
 service type is used to provide the networking layers a hint of the purpose 
 of the request.
 
 @constant NSURLNetworkServiceTypeDefault Is the default value for an NSURLRequest
 when created.  This value should be left unchanged for the vast majority of requests.
 
 @constant NSURLNetworkServiceTypeVoIP Specifies that the request is for voice over IP
 control traffic.
 
 @constant NSURLNetworkServiceTypeVideo Specifies that the request is for video
 traffic.

 @constant NSURLNetworkServiceTypeBackground Specifies that the request is for background
 traffic (such as a file download).

 @constant NSURLNetworkServiceTypeVoice Specifies that the request is for voice data.

*/
// 请求的网络服务类型 
typedef NS_ENUM(NSUInteger, NSURLRequestNetworkServiceType)
{
    NSURLNetworkServiceTypeDefault = 0,	// Standard internet traffic
    NSURLNetworkServiceTypeVoIP = 1,	// Voice over IP control traffic
    NSURLNetworkServiceTypeVideo = 2,	// Video traffic
    NSURLNetworkServiceTypeBackground = 3, // Background traffic
    NSURLNetworkServiceTypeVoice = 4	   // Voice data
};


/*!
    @class NSURLRequest
    
    @abstract An NSURLRequest object represents a URL load request in a
    manner independent of protocol and URL scheme.
    
    @discussion NSURLRequest encapsulates two basic data elements about
    a URL load request:
    <ul>
    <li>The URL to load.
    <li>The policy to use when consulting the URL content cache made
    available by the implementation.
    </ul>
    In addition, NSURLRequest is designed to be extended to support
    protocol-specific data by adding categories to access a property
    object provided in an interface targeted at protocol implementors.
    <ul>
    <li>Protocol implementors should direct their attention to the
    NSURLRequestExtensibility category on NSURLRequest for more
    information on how to provide extensions on NSURLRequest to
    support protocol-specific request information.
    <li>Clients of this API who wish to create NSURLRequest objects to
    load URL content should consult the protocol-specific NSURLRequest
    categories that are available. The NSHTTPURLRequest category on
    NSURLRequest is an example.
    </ul>
    <p>
    Objects of this class are used to create NSURLConnection instances,
    which can are used to perform the load of a URL, or as input to the
    NSURLConnection class method which performs synchronous loads.
*/
// 网络请求对象    
@interface NSURLRequest : NSObject <NSSecureCoding, NSCopying, NSMutableCopying>
{
    @private
    NSURLRequestInternal *_internal;
}

/*! 
    @method requestWithURL:
    @abstract Allocates and initializes an NSURLRequest with the given
    URL.
    @discussion Default values are used for cache policy
    (NSURLRequestUseProtocolCachePolicy) and timeout interval (60
    seconds).
    @param URL The URL for the request.
    @result A newly-created and autoreleased NSURLRequest instance.
*/
// 实例化    
+ (instancetype)requestWithURL:(NSURL *)URL;

/*
    @method supportsSecureCoding
    @abstract Indicates that NSURLRequest implements the NSSecureCoding protocol.
    @result A BOOL value set to YES.
*/
//是否实现了NSSecureCoding协议    
+ (BOOL)supportsSecureCoding;

/*!
    @method requestWithURL:cachePolicy:timeoutInterval:
    @abstract Allocates and initializes a NSURLRequest with the given
    URL and cache policy.
    @param URL The URL for the request. 
    @param cachePolicy The cache policy for the request. 
    @param timeoutInterval The timeout interval for the request. See the
    commentary for the <tt>timeoutInterval</tt> for more information on
    timeout intervals.
    @result A newly-created and autoreleased NSURLRequest instance. 
*/
// 多参数实例化    
+ (instancetype)requestWithURL:(NSURL *)URL cachePolicy:(NSURLRequestCachePolicy)cachePolicy timeoutInterval:(NSTimeInterval)timeoutInterval;

/*! 
    @method initWithURL:
    @abstract Initializes an NSURLRequest with the given URL. 
    @discussion Default values are used for cache policy
    (NSURLRequestUseProtocolCachePolicy) and timeout interval (60
    seconds).
    @param URL The URL for the request. 
    @result An initialized NSURLRequest. 
*/
// 实例化对象方法    
- (instancetype)initWithURL:(NSURL *)URL;

/*! 
    @method initWithURL:
    @abstract Initializes an NSURLRequest with the given URL and
    cache policy.
    @discussion This is the designated initializer for the 
    NSURLRequest class.
    @param URL The URL for the request. 
    @param cachePolicy The cache policy for the request. 
    @param timeoutInterval The timeout interval for the request. See the
    commentary for the <tt>timeoutInterval</tt> for more information on
    timeout intervals.
    @result An initialized NSURLRequest. 
*/
// 实例化对象方法     
- (instancetype)initWithURL:(NSURL *)URL cachePolicy:(NSURLRequestCachePolicy)cachePolicy timeoutInterval:(NSTimeInterval)timeoutInterval NS_DESIGNATED_INITIALIZER;

/*! 
    @method URL
    @abstract Returns the URL of the receiver. 
    @result The URL of the receiver. 
*/
// 请求的url    
@property (nullable, readonly, copy) NSURL *URL;

/*! 
    @method cachePolicy
    @abstract Returns the cache policy of the receiver. 
    @result The cache policy of the receiver. 
*/
// 请求的缓存策略    
@property (readonly) NSURLRequestCachePolicy cachePolicy;

/*! 
    @method timeoutInterval
    @abstract Returns the timeout interval of the receiver.
    @discussion The timeout interval specifies the limit on the idle
    interval alloted to a request in the process of loading. The "idle
    interval" is defined as the period of time that has passed since the
    last instance of load activity occurred for a request that is in the
    process of loading. Hence, when an instance of load activity occurs
    (e.g. bytes are received from the network for a request), the idle
    interval for a request is reset to 0. If the idle interval ever
    becomes greater than or equal to the timeout interval, the request
    is considered to have timed out. This timeout interval is measured
    in seconds.
    @result The timeout interval of the receiver. 
*/
// 请求的超时时长    
@property (readonly) NSTimeInterval timeoutInterval;

/*!
    @method mainDocumentURL
    @abstract The main document URL associated with this load.
    @discussion This URL is used for the cookie "same domain as main
    document" policy. There may also be other future uses.
    See setMainDocumentURL:
    NOTE: In the current implementation, this value is unused by the
    framework. A fully functional version of this method will be available 
    in the future. 
    @result The main document URL.
*/
@property (nullable, readonly, copy) NSURL *mainDocumentURL;

/*!
 @method networkServiceType
 @abstract Returns the NSURLRequestNetworkServiceType associated with this request.
 @discussion  This will return NSURLNetworkServiceTypeDefault for requests that have
 not explicitly set a networkServiceType (using the setNetworkServiceType method).
 @result The NSURLRequestNetworkServiceType associated with this request.
 */
// 请求的网络服务类型 
@property (readonly) NSURLRequestNetworkServiceType networkServiceType NS_AVAILABLE(10_7, 4_0);

/*! 
 @method allowsCellularAccess:
 @abstract returns whether a connection created with this request is allowed to use
 the built in cellular radios (if present).
 @result YES if the receiver is allowed to use the built in cellular radios to
 satify the request, NO otherwise.
 */
// 是否允许通过蜂窝网络访问 
@property (readonly) BOOL allowsCellularAccess  NS_AVAILABLE(10_8, 6_0);

@end



/*! 
    @class NSMutableURLRequest
    
    @abstract An NSMutableURLRequest object represents a mutable URL load
    request in a manner independent of protocol and URL scheme.
    
    @discussion This specialization of NSURLRequest is provided to aid
    developers who may find it more convenient to mutate a single request
    object for a series of URL loads instead of creating an immutable
    NSURLRequest for each load. This programming model is supported by
    the following contract stipulation between NSMutableURLRequest and 
    NSURLConnection: NSURLConnection makes a deep copy of each 
    NSMutableURLRequest object passed to one of its initializers.    
    <p>NSMutableURLRequest is designed to be extended to support
    protocol-specific data by adding categories to access a property
    object provided in an interface targeted at protocol implementors.
    <ul>
    <li>Protocol implementors should direct their attention to the
    NSMutableURLRequestExtensibility category on
    NSMutableURLRequest for more information on how to provide
    extensions on NSMutableURLRequest to support protocol-specific
    request information.
    <li>Clients of this API who wish to create NSMutableURLRequest
    objects to load URL content should consult the protocol-specific
    NSMutableURLRequest categories that are available. The
    NSMutableHTTPURLRequest category on NSMutableURLRequest is an
    example.
    </ul>
*/
// 可变网络请求    
@interface NSMutableURLRequest : NSURLRequest

/*! 
    @method URL
    @abstract Sets the URL of the receiver. 
    @param URL The new URL for the receiver. 
*/
@property (nullable, copy) NSURL *URL;

/*! 
    @method setCachePolicy:
    @abstract The cache policy of the receiver. 
    @param policy The new NSURLRequestCachePolicy for the receiver. 
*/
@property NSURLRequestCachePolicy cachePolicy;

/*! 
    @method setTimeoutInterval:
    @abstract Sets the timeout interval of the receiver.
    @discussion The timeout interval specifies the limit on the idle
    interval allotted to a request in the process of loading. The "idle
    interval" is defined as the period of time that has passed since the
    last instance of load activity occurred for a request that is in the
    process of loading. Hence, when an instance of load activity occurs
    (e.g. bytes are received from the network for a request), the idle
    interval for a request is reset to 0. If the idle interval ever
    becomes greater than or equal to the timeout interval, the request
    is considered to have timed out. This timeout interval is measured
    in seconds.
    @param seconds The new timeout interval of the receiver. 
*/
@property NSTimeInterval timeoutInterval;

/*!
    @method setMainDocumentURL:
    @abstract Sets the main document URL
    @param URL The main document URL.
    @discussion The caller should pass the URL for an appropriate main
    document, if known. For example, when loading a web page, the URL
    of the main html document for the top-level frame should be
    passed.  This main document will be used to implement the cookie
    "only from same domain as main document" policy, and possibly
    other things in the future.
    NOTE: In the current implementation, the passed-in value is unused by the
    framework. A fully functional version of this method will be available 
    in the future. 
*/
@property (nullable, copy) NSURL *mainDocumentURL;

/*!
 @method setNetworkServiceType:
 @abstract Sets the NSURLRequestNetworkServiceType to associate with this request
 @param networkServiceType The NSURLRequestNetworkServiceType to associate with the request.
 @discussion This method is used to provide the network layers with a hint as to the purpose
 of the request.  Most clients should not need to use this method.
 */
@property NSURLRequestNetworkServiceType networkServiceType NS_AVAILABLE(10_7, 4_0);

/*! 
 @method setAllowsCellularAccess
 @abstract sets whether a connection created with this request is allowed to use
 the built in cellular radios (if present).
 @param allow NO if the receiver should not be allowed to use the built in
 cellular radios to satisfy the request, YES otherwise.  The default is YES.
 */
@property BOOL allowsCellularAccess NS_AVAILABLE(10_8, 6_0);

@end



/*!
    @category NSURLRequest(NSHTTPURLRequest) 
    The NSHTTPURLRequest on NSURLRequest provides methods for accessing
    information specific to HTTP protocol requests.
*/
// http请求扩展    
@interface NSURLRequest (NSHTTPURLRequest) 

/*! 
    @method HTTPMethod
    @abstract Returns the HTTP request method of the receiver. 
    @result the HTTP request method of the receiver. 
*/
// 请求方式    
@property (nullable, readonly, copy) NSString *HTTPMethod;

/*! 
    @method allHTTPHeaderFields
    @abstract Returns a dictionary containing all the HTTP header fields
    of the receiver.
    @result a dictionary containing all the HTTP header fields of the
    receiver.
*/
// 请求头信息    
@property (nullable, readonly, copy) NSDictionary<NSString *, NSString *> *allHTTPHeaderFields;

/*! 
    @method valueForHTTPHeaderField:
    @abstract Returns the value which corresponds to the given header
    field. Note that, in keeping with the HTTP RFC, HTTP header field
    names are case-insensitive.
    @param field the header field name to use for the lookup
    (case-insensitive).
    @result the value associated with the given header field, or nil if
    there is no value associated with the given header field.
*/
// 请求头中field对应的信息    
- (nullable NSString *)valueForHTTPHeaderField:(NSString *)field;

/*! 
    @method HTTPBody
    @abstract Returns the request body data of the receiver. 
    @discussion This data is sent as the message body of the request, as
    in done in an HTTP POST request.
    @result The request body data of the receiver. 
*/
// 请求体数据    
@property (nullable, readonly, copy) NSData *HTTPBody;

/*!
    @method HTTPBodyStream
    @abstract Returns the request body stream of the receiver
    if any has been set
    @discussion The stream is returned for examination only; it is
    not safe for the caller to manipulate the stream in any way.  Also
    note that the HTTPBodyStream and HTTPBody are mutually exclusive - only
    one can be set on a given request.  Also note that the body stream is
    preserved across copies, but is LOST when the request is coded via the 
    NSCoding protocol
    @result The request body stream of the receiver.
*/
// 请求体数据流    
@property (nullable, readonly, retain) NSInputStream *HTTPBodyStream;

/*! 
    @method HTTPShouldHandleCookies
    @abstract Determine whether default cookie handling will happen for 
    this request.
    @discussion NOTE: This value is not used prior to 10.3
    @result YES if cookies will be sent with and set for this request; 
    otherwise NO.
*/
@property (readonly) BOOL HTTPShouldHandleCookies;

/*!
 @method HTTPShouldUsePipelining
 @abstract Reports whether the receiver is not expected to wait for the
 previous response before transmitting.
 @result YES if the receiver should transmit before the previous response
 is received.  NO if the receiver should wait for the previous response
 before transmitting.
 */
@property (readonly) BOOL HTTPShouldUsePipelining NS_AVAILABLE(10_7, 4_0);

@end



/*!
    @category NSMutableURLRequest(NSMutableHTTPURLRequest) 
    The NSMutableHTTPURLRequest on NSMutableURLRequest provides methods
    for configuring information specific to HTTP protocol requests.
*/
// 可变网络请求对象的http访问扩展        
@interface NSMutableURLRequest (NSMutableHTTPURLRequest) 

/*! 
    @method HTTPMethod:
    @abstract Sets the HTTP request method of the receiver. 
    @param method the new HTTP request method for the receiver.
*/
@property (copy) NSString *HTTPMethod;

/*! 
    @method allHTTPHeaderFields:
    @abstract Sets the HTTP header fields of the receiver to the given
    dictionary.
    @discussion This method replaces all header fields that may have
    existed before this method call. 
    <p>Since HTTP header fields must be string values, each object and
    key in the dictionary passed to this method must answer YES when
    sent an <tt>-isKindOfClass:[NSString class]</tt> message. If either
    the key or value for a key-value pair answers NO when sent this
    message, the key-value pair is skipped.
    @param headerFields a dictionary containing HTTP header fields.
*/
@property (nullable, copy) NSDictionary<NSString *, NSString *> *allHTTPHeaderFields;

/*! 
    @method setValue:forHTTPHeaderField:
    @abstract Sets the value of the given HTTP header field.
    @discussion If a value was previously set for the given header
    field, that value is replaced with the given value. Note that, in
    keeping with the HTTP RFC, HTTP header field names are
    case-insensitive.
    @param value the header field value. 
    @param field the header field name (case-insensitive). 
*/
- (void)setValue:(nullable NSString *)value forHTTPHeaderField:(NSString *)field;

/*! 
    @method addValue:forHTTPHeaderField:
    @abstract Adds an HTTP header field in the current header
    dictionary.
    @discussion This method provides a way to add values to header
    fields incrementally. If a value was previously set for the given
    header field, the given value is appended to the previously-existing
    value. The appropriate field delimiter, a comma in the case of HTTP,
    is added by the implementation, and should not be added to the given
    value by the caller. Note that, in keeping with the HTTP RFC, HTTP
    header field names are case-insensitive.
    @param value the header field value. 
    @param field the header field name (case-insensitive). 
*/
- (void)addValue:(NSString *)value forHTTPHeaderField:(NSString *)field;

/*! 
    @method HTTPBody:
    @abstract Sets the request body data of the receiver.
    @discussion This data is sent as the message body of the request, as
    in done in an HTTP POST request.
    @param data the new request body data for the receiver.
*/
@property (nullable, copy) NSData *HTTPBody;

/*!
    @method HTTPBodyStream:
    @abstract Sets the request body to be the contents of the given stream. 
    @discussion The provided stream should be unopened; the request will take
    over the stream's delegate.  The entire stream's contents will be 
    transmitted as the HTTP body of the request.  Note that the body stream
    and the body data (set by setHTTPBody:, above) are mutually exclusive 
    - setting one will clear the other. 
    @param inputStream the new input stream for use by the receiver
*/
@property (nullable, retain) NSInputStream *HTTPBodyStream;

/*!
    @method HTTPShouldHandleCookies
    @abstract Decide whether default cookie handling will happen for 
    this request.
    @param YES if cookies should be sent with and set for this request; 
    otherwise NO.
    @discussion The default is YES - in other words, cookies are sent from and 
    stored to the cookie manager by default.
    NOTE: In releases prior to 10.3, this value is ignored
*/
@property BOOL HTTPShouldHandleCookies;

/*!
 @method HTTPShouldUsePipelining
 @abstract Sets whether the request should not wait for the previous response 
 before transmitting.
 @param YES if the receiver should transmit before the previous response is
 received.  NO to wait for the previous response before transmitting.
 @discussion Calling this method with a YES value does not guarantee HTTP 
 pipelining behavior.  This method may have no effect if an HTTP proxy is
 configured, or if the HTTP request uses an unsafe request method (e.g., POST
 requests will not pipeline).  Pipelining behavior also may not begin until
 the second request on a given TCP connection.  There may be other situations
 where pipelining does not occur even though YES was set.
 HTTP 1.1 allows the client to send multiple requests to the server without
 waiting for a response.  Though HTTP 1.1 requires support for pipelining,
 some servers report themselves as being HTTP 1.1 but do not support
 pipelining (disconnecting, sending resources misordered, omitting part of
 a resource, etc.).
 */
@property BOOL HTTPShouldUsePipelining NS_AVAILABLE(10_7, 4_0);

@end

NS_ASSUME_NONNULL_END
