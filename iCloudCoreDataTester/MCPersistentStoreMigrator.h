//
// This class can migrate SQLite persistent stores from one URL to another, like
// the NSPersistentStoreCoordinator class's migratePersistentStore:... method.
// It can thus be used to seed iCloud with an existing store's data.
// The advantage of this class over NSPersistentStoreCoordinator is that it does
// not pull the whole store into memory. You can do the migration in batches, with
// regular saves. The developer controls batch size, and you can 'snip' relationships
// to restrict a sub-migration to one part of the object graph.
//

#import <CoreData/CoreData.h>
#import <Foundation/Foundation.h>

@class MCManagedObjectContext;

@interface MCPersistentStoreMigrator : NSObject

@property (nonatomic, readonly) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, readonly) NSURL *destinationStoreURL, *sourceStoreURL;
@property (nonatomic, readwrite, copy) NSDictionary *sourceStoreOptions, *destinationStoreOptions;

-(id)initWithManagedObjectModel:(NSManagedObjectModel *)model sourceStoreURL:(NSURL *)sourceURL destinationStoreURL:(NSURL *)destinationURL;

// Invoke these at the beginning and end of the migration.
// All relationship snips and sub-migrations should fall in between.
-(void)beginMigration;
-(void)endMigration;

// Performs a sub-migration of one entity, and all connected objects. You can choose
// a batch size for fetching, and whether or not to save after each batch.
// If you want to do several sub-migrations before saving, only pass YES for the save:
// argument of the last in the series.
-(BOOL)migrateEntityWithName:(NSString *)entityName batchSize:(NSUInteger)batchSize save:(BOOL)save error:(NSError **)error;

// Snipping a relationship means it will not be traversed during migration. 
// You can use this to restrict a sub-migration to just part of the object graph.
// Note that the object graph must be valid in order to save. Usually this means you
// should only snip optional relationships.
// For relationships with an inverse, the 'snipped' relationship will automatically
// get set when the inverse relationship is set. If there is no inverse relationship,
// you can use the stitchRelationship: method to explicitly set the snipped relationship,
// but this is generally not necessary.
-(void)snipRelationship:(NSString *)relationshipKey inEntity:(NSString *)entityName;
-(BOOL)stitchRelationship:(NSString *)relationshipName inEntity:(NSString *)entityName save:(BOOL)save error:(NSError **)error;

@end
