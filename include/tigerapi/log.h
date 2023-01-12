//
// Created by sukai on 2023/1/12.
//

#ifndef TIGERAPI_LOG_H
#define TIGERAPI_LOG_H

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;

namespace logging = boost::log;

#define LOGGER BOOST_LOG_TRIVIAL



#endif //TIGERAPI_LOG_H
