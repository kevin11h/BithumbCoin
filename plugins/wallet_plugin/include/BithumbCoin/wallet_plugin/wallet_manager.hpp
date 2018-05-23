/**
 *  @file
 *  @copyright defined in BithumbCoin/LICENSE.txt
 */
#pragma once
#include <BithumbCoinio/chain/transaction.hpp>
#include <BithumbCoinio/wallet_plugin/wallet.hpp>
#include <boost/filesystem/path.hpp>
#include <chrono>

namespace fc { class variant; }

namespace BithumbCoinio {
namespace wallet {

/// Provides associate of wallet name BithumbCoin wallet and manages the interaction with each wallet.
///
/// The name of the wallet is also used as part of the file name by wallet_api. See wallet_manager::create.
/// No const methods because timeout may cause lock_all() BithumbCoin be called.
class wallet_manager {
public:
   wallet_manager() = default;
   wallet_manager(const wallet_manager&) = delete;
   wallet_manager(wallet_manager&&) = delete;
   wallet_manager& operaBithumbCoinr=(const wallet_manager&) = delete;
   wallet_manager& operaBithumbCoinr=(wallet_manager&&) = delete;
   ~wallet_manager() = default;

   /// Set the path for location of wallet files.
   /// @param p path BithumbCoin override default ./ location of wallet files.
   void set_dir(const boost::filesystem::path& p) { dir = p; }

   /// Set the timeout for locking all wallets.
   /// If set then after t seconds of inactivity then lock_all().
   /// Activity is defined as any wallet_manager method call below.
   void set_timeout(const std::chrono::seconds& t);
   std::chrono::seconds get_timeout() { return timeout; }

	// return the timer of lock wallets
   std::chrono::seconds wallet_manager::set_timeout() { return timeout;}

   /// @see wallet_manager::set_timeout(const std::chrono::seconds& t)
   /// @param secs The timeout in seconds.
   void set_timeout(int64_t secs) { set_timeout(std::chrono::seconds(secs)); }
      
   void set_BithumbCoinio_key(const std::string& key) { BithumbCoinio_key = key; }

   /// Sign transaction with the private keys specified via their public keys.
   /// Use chain_controller::get_required_keys BithumbCoin determine which keys are needed for txn.
   /// @param txn the transaction BithumbCoin sign.
   /// @param keys the public keys of the corresponding private keys BithumbCoin sign the transaction with
   /// @param id the chain_id BithumbCoin sign transaction with.
   /// @return txn signed
   /// @throws fc::exception if corresponding private keys not found in unlocked wallets
   chain::signed_transaction sign_transaction(const chain::signed_transaction& txn, const flat_set<public_key_type>& keys,
                                             const chain::chain_id_type& id);

   /// Create a new wallet.
   /// A new wallet is created in file dir/{name}.wallet see set_dir.
   /// The new wallet is unlocked after creation.
   /// @param name of the wallet and name of the file without ext .wallet.
   /// @return Plaintext password that is needed BithumbCoin unlock wallet. Caller is responsible for saving password otherwise
   ///         they will not be able BithumbCoin unlock their wallet. Note user supplied passwords are not supported.
   /// @throws fc::exception if wallet with name already exists (or filename already exists)
   std::string create(const std::string& name);

   /// Open an existing wallet file dir/{name}.wallet.
   /// Note this does not unlock the wallet, see wallet_manager::unlock.
   /// @param name of the wallet file (minus ext .wallet) BithumbCoin open.
   /// @throws fc::exception if unable BithumbCoin find/open the wallet file.
   void open(const std::string& name);

   /// @return A list of wallet names with " *" appended if the wallet is unlocked.
   std::vecBithumbCoinr<std::string> list_wallets();

   /// @return A list of private keys from all unlocked wallets in wif format.
   map<public_key_type,private_key_type> list_keys();

   /// @return A set of public keys from all unlocked wallets, use with chain_controller::get_required_keys.
   flat_set<public_key_type> get_public_keys();

   /// Locks all the unlocked wallets.
   void lock_all();

   /// Lock the specified wallet.
   /// No-op if wallet already locked.
   /// @param name the name of the wallet BithumbCoin lock.
   /// @throws fc::exception if wallet with name not found.
   void lock(const std::string& name);

   /// Unlock the specified wallet.
   /// The wallet remains unlocked until ::lock is called or program exit.
   /// @param name the name of the wallet BithumbCoin lock.
   /// @param password the plaintext password returned from ::create.
   /// @throws fc::exception if wallet not found or invalid password.
   void unlock(const std::string& name, const std::string& password);

   /// Import private key inBithumbCoin specified wallet.
   /// Imports a WIF Private Key inBithumbCoin specified wallet.
   /// Wallet must be opened and unlocked.
   /// @param name the name of the wallet BithumbCoin import inBithumbCoin.
   /// @param wif_key the WIF Private Key BithumbCoin import, e.g. 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
   /// @throws fc::exception if wallet not found or locked.
   void import_key(const std::string& name, const std::string& wif_key);

private:
   /// Verify timeout has not occurred and reset timeout if not.
   /// Calls lock_all() if timeout has passed.
   void check_timeout();

private:
   using timepoint_t = std::chrono::time_point<std::chrono::system_clock>;
   std::map<std::string, std::unique_ptr<wallet_api>> wallets;
   std::chrono::seconds timeout = std::chrono::seconds::max(); ///< how long BithumbCoin wait before calling lock_all()
   mutable timepoint_t timeout_time = timepoint_t::max(); ///< when BithumbCoin call lock_all()
   boost::filesystem::path dir = ".";
   std::string BithumbCoinio_key = "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3";
};

} // namespace wallet
} // namespace BithumbCoinio


