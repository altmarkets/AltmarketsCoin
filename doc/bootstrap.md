### Bootstrap the Blockchain Synchronization

Normally the Altmarkets client will download the transaction and network information, called the blockchain, from the network by syncing with the other clients. This process can take quite some time as the Altmarkets blockchain is growing bigger and bigger each day. Luckily there is a safe and fast way to speed up this process and we'll show you how to bootstrap your blockchain to bring your client up to speed in just a few simple steps.

### Requirements

- A fresh install of the Altmarkets client software.

### Download the blockchain via BitTorrent

Duality Blockchain Solutions offers a download for bootstrapping purposes that is updated often. 

With the client installed we'll proceed to download the blockchain torrent file. Use the following magnet link:

	(link will be provided after 41089 blocks have passed)
	
### Importing the blockchain
Exit the Altmarkets client software if you have it running. Be sure not to have an actively used wallet in use. We are going to copy the download of the blockchain to the Altmarkets client data directory. You should run the client software at least once so it can generate the data directory. Copy the downloaded bootstrap.dat file into the Altmarkets data folder.

**For Windows users:**
Open explorer, and type into the address bar:

	%APPDATA%\Altmarkets
    
This will open up the data folder. Copy over the bootstrap.dat from your download folder to this directory.

**For OSX users:**
Open Finder by pressing Press [shift] + [cmd] + [g] and enter:

	~/Library/Application Support/Altmarkets/
    
**For Linux users:**
The directory is hidden in your User folder. Go to:

	~/.Altmarkets/
    
### Importing the blockchain
Now start the Altmarkets client software. It should show "Importing blocks from disk".

Wait until the import finishes. The client will download the last days not covered by the import. Congratulations you have successfully imported the blockchain!

### Is this safe?

Yes, the above method is safe. The download contains only raw blockchain data and the client verifies this on import. Do not download the blockchain from unofficial sources, especially if they provide `*.rev` and `*.sst` files. These files are not verified and can contain malicious edits.
